// AArch64 mode
 
// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make _start global.
.globl _start
 
 .org 0x80000
_start:
    mrs    x0, mpidr_el1        
    and    x0, x0,#0xFF        // Check processor id
    cbz    x0, _bootstrap      // Hang for all non-primary CPU
    b      _hang

_hang:
    b _hang

_bootstrap:
    // set stack before our code
    ldr     x5, =_start
    mov     sp, x5
 
    // clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
3:  cbz     w6, 4f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 3b

    // The processor ID is still in x0, and we don't have any boot information
    // for core_entry() on this platform, so zero out the x1 register.
    mov     x1, xzr
 
    // jump to C code, should not return
4:  bl      core_entry
    // for failsafe, halt this core too
    b _hang
