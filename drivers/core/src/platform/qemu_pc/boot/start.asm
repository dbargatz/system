global start
global ist1_stack_top
global ist2_stack_top
global kernel_heap_bottom
global kernel_heap_top
global gdt64

extern core_entry
extern start_init_array
extern end_init_array

section .text
bits 32
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Entry point to System. Any Multiboot-compliant bootloader will begin 
;; executing code here, since the linker script places this code immediately 
;; after the multiboot header. As such, this function must stay at the very top
;; of this file so the bootloader executes the start function first. When we 
;; enter here, presuming the bootloader did its job, we have the following 
;; conditions:
;;    1. In protected mode (32-bit)
;;    2. Paging is disabled
;;    3. A 32-bit GDT is loaded
;;    4. EAX contains the Multiboot magic value
;;    5. EBX contains the 32-bit physical address of the Multiboot info struct
start:
    ;; Set up the stack pointer.
    mov esp, stack_top

    ;; By specification, the bootloader should have placed a 32-bit
    ;; pointer to the Multiboot information structure in EBX. This
    ;; information structure contains a wealth of platform info that
    ;; was determined by the bootloader, including a physical memory
    ;; map. We need to pass this into core_entry, but there are two
    ;; problems:
    ;;  1. We need to use EBX in the meantime.
    ;;  2. core_entry is 64-bit code, so it expects a 64-bit pointer.
    ;; The solution is to push 4 bytes (32 bits) of zeroes on the
    ;; stack followed by the 32-bit (4 byte) pointer in EBX, which
    ;; effectively creates a 64-bit pointer that has been extended
    ;; from the 32-bit pointer. In 64-bit mode, we'll pop the full 8
    ;; bytes (64 bits) off of the stack to pass to core_entry, so
    ;; it looks like a real 64-bit pointer.
    push 0x00000000
    push ebx

    ;; Perform the Multiboot check to ensure the kernel was started
    ;; from a Multiboot-compliant bootloader. This won't return if
    ;; the bootloader isn't Multiboot-compliant.
    call check_multiboot

    ;; Perform the CPUID check - verify that the processor supports
    ;; the CPUID instruction. Since CPUID is needed to determine if
    ;; long mode is supported, we make this check before attempting
    ;; to check if long mode is supported.
    call check_cpuid

    ;; Perform the long mode check - verify that the processor
    ;; supports long mode (64-bit). As the code for the kernel
    ;; is 64-bit, we need to enter long mode, but need to verify the
    ;; processor supports it before trying to enter long mode.
    call check_long_mode

    ;; Verify and enable SSE so we can use floating-point arithmetic.
    call enable_sse

    ;; Now that we've verified the processor supports long mode, we
    ;; need to set up the page tables before we can proceed.
    call set_up_page_tables

    ;; The page tables are initialized and the first gibibyte of
    ;; memory is identity mapped, so enable paging and enter long
    ;; mode.
    call enable_paging

    ;; At this point, we're in long mode, but we're in the 32-bit
    ;; submode of long mode called "compatibility mode". This means
    ;; we still can't use 64-bit instructions. Thanks to legacy cruft
    ;; from the x86 architecture, we need to set up a Global Descriptor
    ;; Table (GDT), which was used for segmentation in the past. Our
    ;; Multiboot-compliant bootloader set up a 32-bit GDT for us before
    ;; it handed off control to the start function, but we need to
    ;; setup and transition to a 64-bit GDT. We use the lgdt instruction
    ;; to load the GDT into the processor, providing it with the special
    ;; GDT pointer structure (described below in the .rodata section,
    ;; where the GDT is defined).
    lgdt[gdt64.pointer]

    ;; We're still not quite into 64-bit long mode; the segment selector
    ;; registers are still pointing at GDT entries from the 32-bit GDT
    ;; set up by the bootloader. We need to point the stack, data, and
    ;; extra selector registers to the data segment entry in the 64-bit
    ;; GDT. We can't load the code selector (cs) the same way - we'll
    ;; handle that after we load the data segment entry into ss, ds, and
    ;; es.
    mov ax, gdt64.ring0_data
    mov ss, ax
    mov ds, ax
    mov es, ax

    ;; To (finally!) move into 64-bit long mode, we need to perform a far
    ;; jump into 64-bit code; this will load the code selector (cs)
    ;; register with the proper code segment entry in the GDT, since (as
    ;; mentioned above), we can't just load cs with an arbitrary memory
    ;; address. When this jump completes, we'll be in 64-bit long mode!
    jmp gdt64.ring0_code:long_mode_start
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bits 64
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; The first 64-bit function called during kernel startup. The start
;; label in start.asm will perform a far jump to this function in
;; order to load the code selector (cs) register with the 64-bit Global
;; Descriptor Table (GDT) entry for the code segment, and place the
;; processor into 64-bit long mode (rather than long mode's compatibility
;; submode).
long_mode_start:
    mov rbx, start_init_array
    jmp .call_ctors_loop

.call_ctor:
    call [rbx]
    add rbx, 8

.call_ctors_loop:
    cmp rbx, end_init_array
    jb .call_ctor

    ;; Pop the Multiboot 2 information structure off the stack into RDI. By the
    ;; System V AMD64 ABI calling convention, RDI contains the first integer
    ;; argument to a function call and RSI contains the second integer argument.
    ;; The core_entry() function expects a processor ID as the first argument
    ;; and a pointer to any platform-specific boot information as the second
    ;; argument; because this is the bootstrap core, pass zero as the processor
    ;; ID and pop the Multiboot 2 info struct pointer off the stack into RSI.
    xor rdi, rdi
    pop rsi

    ;; Load RAX with the address of core_entry and call it!
    lea rax, [core_entry]
    call rax

    ;; Halt the processor.
    hlt
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bits 32
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Prints `ERR: ` and the given error code to the screen and hangs.
;; Parameter: error code (in ASCII) in AL
print32:
    mov dword [0xb8000], 0x4F524F45
    mov dword [0xb8004], 0x4F3A4F52
    mov dword [0xb8008], 0x4F204F20
    mov byte  [0xb800a], al
    hlt
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Verifies that the kernel was loaded by a multiboot-compliant
;; bootloader via the initial value placed in EAX by the bootloader.
;; Prints error "0" and hangs if the bootloader wasn't multiboot
;; compliant.
check_multiboot:
    cmp eax, 0x36D76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    jmp print32
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Verifies that the processor supports the CPUID instruction. If it
;; doesn't, prints error "1" and hangs.
check_cpuid:
    ;; Copy the FLAGS register into EAX via the stack.
    pushfd
    pop eax

    ;; Copy the FLAGS register to ECX as well for comparing later.
    mov ecx, eax

    ;; Flip the "CPU ID supported" flag (bit 21).
    xor eax, 1 << 21

    ;; Copy EAX to the FLAGS register via the stack again.
    push eax
    popfd

    ;; Copy the FLAGS register back into EAX - the "CPU ID supported"
    ;; bit will still be 1 if the CPUID instruction is supported. If
    ;; it isn't supported, then the 21st bit will be reset to 0,
    ;; indicating the CPUID isn't supported.
    pushfd
    pop eax

    ;; Reset the FLAGS register back to its original state, before the
    ;; 21st bit was set to 1.
    push ecx
    popfd

    ;; Compare ECX and EAX. If they're the same - that is, the 21st bit
    ;; for "CPU ID supported" is 0 - then CPUID isn't a supported
    ;; instruction. If the bit is 1, then CPUID is supported. If it
    ;; isn't supported, indicate error "1".
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp print32
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Verifies that the processor supports long mode (64-bit). This function
;; relies on the CPUID instruction, so check_cpuid should be called
;; before this function is called. Prints error "2" and hangs if long
;; mode isn't supported.
check_long_mode:
    ;; CPUID works by taking a parameter via EAX and returning values
    ;; in various registers (depending on the parameter provided).
    ;; As the long mode bit is considered extended processor info,
    ;; we first need to verify that the CPUID instruction on this
    ;; processor even supports extended processor info. To do so, we pass
    ;; 0x80000000 to CPUID, which will return the highest set of extended
    ;; info supported by the processor. Since the long mode bit is
    ;; in the first "set" of extended processor attributes (the set
    ;; identified by 0x80000001 in EAX), we need to verify the value
    ;; returned by CPUID is at least 0x80000001. If the processor doesn't
    ;; support extended processor info of at least set 0x80000001, it
    ;; doesn't support long mode, so error out.
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode

    ;; At this point, we know the processor supports extended processor
    ;; info. If we invoke CPUID with 0x80000001 in EAX, various info and
    ;; feature bits for the processor will be returned in ECX and EDX.
    ;; The long mode supported bit is bit 29 of the value returned in
    ;; EDX; if this bit is set in EDX, then long mode is supported and
    ;; we can continue the boot process. If the bit isn't set, long mode
    ;; isn't supported, so error out.
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
.no_long_mode:
    mov al, "2"
    jmp print32
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Identity maps the first gigibyte (1GiB) of memory, meaning the first
;; gibibyte of virtual addresses map to the first gibibyte of physical
;; addresses. It does so using 512 2MiB huge pages, meaning only the
;; first entry of the P4 and P3 tables are valid, and there is a single
;; P2 table that identity maps the first 512 2MiB pages. This memory is
;; only accessible from ring 0 (supervisor mode); ring 3 (user mode)
;; code must have appropriate page tables created for them.
set_up_page_tables:
    ;; Initialize the first entry of the P4 table to contain the physical
    ;; address of the first (and only) P3 table. Note that the entry in
    ;; the P4 table for the P3 table has marked the P3 table's page as
    ;; present, user-inaccessible, and writeable, as indicated by the
    ;; ORing of EAX with 0b011.
    mov eax, p3_table
    or eax, 0b011
    mov [p4_table], eax
    ;; Initialize the first entry of the P3 table to contain the physical
    ;; address of the first (and only) P2 table. Note that the entry in
    ;; the P3 table for the P2 table has marked the P2 table's page as
    ;; present, user-inaccessible and writeable, as indicated by the
    ;; ORing of EAX with 0b011.
    mov eax, p2_table
    or eax, 0b011
    mov [p3_table], eax

    ;; Initialize the counter for the loop below. ECX contains the
    ;; entry number in the P2 table we're currently on in the loop.
    mov ecx, 0

    ;; This loop identity maps the first 512 2MiB pages so that the
    ;; virtual address and physical address are the same number.
.map_p2_table:
    ;; Put 2MiB in EAX.
    mov eax, 0x200000
    ;; Multiply EAX (which contains 2MiB) by ECX, and store the result
    ;; in EAX. EAX now contains the physical starting address of the
    ;; 2MiB page the current entry in the P2 table should point at.
    mul ecx
    ;; Mark the entry as present, user-inaccessible, writeable, and
    ;; huge. While EAX does contain the physical starting address of
    ;; the page, the bottom 12 bits of the address are used as flag
    ;; bits for the entry. Setting the huge bit is what marks the page
    ;; as 2MiB - since this entry is in the P2 table, the huge bit
    ;; indicates the page should be 2MiB.
    or eax, 0b10000011
    ;; Put the address/entry we computed in EAX into the P2 table at
    ;; the proper location. ECX contains the current entry we're
    ;; looking at in this loop iteration, 8 is the number of bytes each
    ;; entry is, so ECX * 8 is the address of the entry in P2.
    mov [p2_table + ecx *8], eax

    ;; Increment ECX to move onto the next entry in the P2 table. If
    ;; ECX is now 512, the whole P2 table is mapped, so don't continue
    ;; looping - just return.
    inc ecx
    cmp ecx, 512
    jne .map_p2_table
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Turns paging on by loading the P4 table's address into CR3, enabling
;; PAE via the CR4 register, setting the long mode bit in the EFER
;; Model-Specific Register (MSR), and turning paging on via the paging
;; bit in CR0.
enable_paging:
    ;; Load the address of the P4 table into CR3. CR3 can only be
    ;; loaded via a register, so we put the address of the P4 table
    ;; into EAX before loading it into CR3. On Intel processors, CR3 is
    ;; the control register containing the address of the top-level
    ;; page table, so CR3 must contain P4's address to properly do
    ;; virtual-to-physical address translation.
    mov eax, p4_table
    mov cr3, eax

    ;; Physical Address Extensions (PAE) needs to be enabled for long
    ;; mode paging to be enabled, since long mode is technically an
    ;; extension to PAE. The PAE bit is the 5th bit in the CR4
    ;; register, so manipulate CR4 via EAX to turn on PAE.
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ;; The long mode bit lives in a Model Specific Register (MSR) called
    ;; the Extended Feature Enable Register (EFER). Its identifier is
    ;; 0xC0000080. Using the rdmsr instruction, a MSR can be read into
    ;; EDX:EAX based on the MSR identifier specified in ECX. The long
    ;; mode bit is the 9th bit in the EFER, and NXE (which enables no-
    ;; execute capabilities on the processor) is the 12th bit, so set
    ;; them and write the byte back into the EFER via the wrmsr
    ;; instruction.
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    or eax, 1 << 11
    wrmsr

    ;; The paging bit, the bit that enables paging in the processor so it
    ;; can operate in long mode, is the final bit we need to set before
    ;; we are in long mode. It's the 31st bit in the CR0 register, so
    ;; just like with enabling PAE, we set the bit via EAX and put it
    ;; back into CR0.
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ;; At this point, we're in long mode, but in compatibility mode of
    ;; long mode.
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Verifies the minimum level of SSE support (SSE3) and enables SSE. Note
;; that once SSE is enabled, code is capable of using floating-point
;; (IEEE754) arithmetic, but can also cause numeric/floating-point
;; exceptions. See Intel Software Developer's Manual Volume 3A, May
;; 2020, page 6-52: Interrupt 19 for more information. If SSE is not
;; supported, prints error "3" and hangs.
enable_sse:
    ;; All the SSE support bits are in CPUID page 1.
    mov eax, 1
    cpuid

    ;; Ensure SSE is supported (EDX bit 25).
    test edx, 1 << 25
    jz .no_sse

    ;; Ensure SSE2 is supported (EDX bit 26).
    test edx, 1 << 26
    jz .no_sse

    ;; Clear CR0.EM (coprocessor emulation, bit 2) and set CR0.MP (coprocessor
    ;; monitoring, bit 1).
    mov eax, cr0
    and eax, ~(1 << 2)
    or eax, 1 << 1
    mov cr0, eax

    ;; Set CR4.OSFXSR (OS supports FXSAVE/FXRSTOR, bit 9) and CR4.OSXMMEXCPT
    ;; (OS supports unmasked SSE exceptions, bit 10).
    mov eax, cr4
    or eax, 1 << 9
    or eax, 1 << 10
    mov cr4, eax

    ret
.no_sse:
    mov al, "3"
    jmp print32
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .bss
align 4096
;; Reserved space for the P4 page table (in Intel parlance, the Page-Map
;; Level-4 Table [PML4]). The 4096-byte reservation allows for a 512-
;; entry table, with each entry being 8 bytes. Each entry points to an
;; entire P3 table. This is the top-level page table, and the address of
;; this table in memory will be placed in register CR3.
p4_table:
    resb 4096
;; Reserved space for the P3 page table (in Intel parlance, the Page-
;; Directory Pointer Table [PDP]). The 4096-byte reservation allows for
;; a 512-entry table, with each entry being 8 bytes. Each entry points
;; to an entire P2 table.
p3_table:
    resb 4096
;; Reserved space for the P2 page table (in Intel parlance, the Page-
;; Directory Table [PD]). The 4096-byte reservation allows for a 512-
;; entry table, with each entry being 8 bytes. Each entry contains the
;; physical address of a 2MiB page in memory. If we were using 4Kb pages,
;; there would be a P1 Page Table level, but since we're using 2MiB huge
;; pages, P2 is the last level in the page hierarchy.
p2_table:
    resb 4096

;; The initial stack for the functions in the boot code. Only 32 kilobytes,
;; but not much is needed - these functions don't have deep call trees,
;; nor do they push/pop much for local variables, other than text objects for
;; log messages.
stack_bottom:
    resb 32768
stack_top:

;; Known-good stack, only used for PANIC/ASSERT calls and loaded into the TSS
;; as Interrupt Stack Table (IST) 1. This can be shallow as it should only be
;; switched to when a PANIC/ASSERT or undefined opcode (as a side effect of the
;; PANIC/ASSERT implementation) occurs.
ist1_stack_bottom:
    resb 2048
ist1_stack_top:

;; Known-good stack, only used for unhandled interrupt vectors and loaded into
;; the TSS as Interrupt Stack Table (IST) 2. This can be shallow as it should
;; only be switched to when an unhandled interrupt vector is triggered.
ist2_stack_bottom:
    resb 2048
ist2_stack_top:

;; TODO
kernel_heap_bottom:
    resb 131070
kernel_heap_top:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
section .rodata
;; TODO: Update this comment for usermode
;; The 64-bit Global Descriptor Table (GDT), to be loaded into the
;; processor via the lgdt instruction after we've entered long mode. Each
;; GDT entry is 64 bits, and there are three minimum required entries for
;; the GDT. The first entry is the null entry - it's 64 bits of zeroes.
;; The second entry is for the code segment, which has the read/write
;; bit, the executable bit, the descriptor type bit, the present bit,
;; and the 64-bit code segment bit all set to 1. The third entry is for
;; the data segment, and it has the read/write bit, the descriptor type
;; bit, and the present bit set to 1. These bits are all that's required,
;; as the GDT isn't really used in long mode; rather, the base and limit
;; fields of each entry are set to 0, meaning each entry (representing
;; a memory segment) spans ALL of memory. Therefore, all of memory is
;; in both the data segment and the code segment.
;; NOTE: While the RW bit is set for the code segment, it's a bit of a 
;; misnomer - really, it means that the segment is readable, but is 
;; never writeable, as it's a code segment. Data segments, on the other
;; hand, are always readable - setting the RW bit means the segment is
;; writeable.
gdt64:
.null_1: equ $-gdt64 ; first entry in GDT must always be NULL.
    dq 0
.ring0_code: equ $-gdt64
;;     64b cs    present   priv level   desctype    exec      r/w     accessed
    dq (1<<53) | (1<<47)              | (1<<44) | (1<<43) | (1<<41)
.ring0_data: equ $-gdt64
    dq           (1<<47)              | (1<<44)           | (1<<41)
.null_2: equ $-gdt64 ; required for syscall/sysret implementation; IA32_STAR MSR has a weird format.
    dq 0
.ring3_data: equ $-gdt64
    dq           (1<<47) | (3 << 45)  | (1<<44)           | (1<<41)
.ring3_code: equ $-gdt64
;;     64b cs    present   priv level   desctype    exec      r/w
    dq (1<<53) | (1<<47) | (3 << 45)  | (1<<44) | (1<<43) | (1<<41)
.tss: equ $-gdt64 ; NOTE: 64-bit TSS descriptor is 16 bytes
    dq 0
    dq 0
;; The GDT pointer is a special data structure expected by the lgdt
;; instruction. This structure  contains the length of the GDT in bytes
;; as the first word (2 bytes), and the next 8 bytes specify the actual
;; address of the GDT in memory.
.pointer:
    dw $ - gdt64 - 1
    dq gdt64
