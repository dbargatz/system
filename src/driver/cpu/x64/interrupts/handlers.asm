extern this_core
extern interrupt_entry
global jump_usermode

section .text
bits 64

interrupt_handler_generic:
    ;; TODO: save SSE registers (XMM; FXSAVE/FXRSTOR?)
    ;; TODO: why push in this order? 
    ;; TODO: Clear direction flag for SysV ABI?
    ;; TODO: push segment regs?
    push rdi
    push rsi
    push rdx
    push rcx
    push rax
    push r8
    push r9
    push r10
    push r11
    push rbx
    push r12
    push r13
    push r14
    push r15
.dispatch:
    ;; Pass the stack frame as the only argument.
    mov rdi, rsp
    lea rax, [interrupt_entry]
    call rax
.restore_registers:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop r11
    pop r10
    pop r9
    pop r8
    pop rax
    pop rcx
    pop rdx
    pop rsi
    mov rdi, [rsp+8]
    cmp rdi, 17
    ja .no_err_exit  ;; if it's above 17, can't have pushed an error code.
    cmp rdi, 8
    jb .no_err_exit  ;; if it's below 8, can't have pushed an error code.
    ; 8-17: YES 8,    10, 11, 12, 13, 14,        17
    ;       NO     9,                     15, 16,
    cmp rdi, 9       ;; From 8-17 inclusive, only 9, 15, and 16 do not push an error code.
    je .no_err_exit
    cmp rdi, 15
    je .no_err_exit
    cmp rdi, 16
    je .no_err_exit
    ;; If we reach here, we must have one of the error-pushing exceptions.
    pop rdi
    add rsp, 0x08
    iretq
.no_err_exit:
    pop rdi
    add rsp, 0x10
    iretq

;; Function pointer is in rdi
jump_usermode:
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ;; ss is handled by the iretq below.
    ;; TODO: should all gp registers get zeroed before heading to usermode?

    mov rax, rsp
    push 0x23
    push rax
    pushf
    push 0x1B
    push rdi
    xor rdi, rdi
    iretq

;; NEED: what is my vector number?
%macro GENERATE_HANDLER_NO_ERR 1
global interrupt_handler_%1

interrupt_handler_%1:
    ;; Push a dummy error code to keep the stack consistent.
    push 0
    push %1
    jmp interrupt_handler_generic
%endmacro

%macro GENERATE_HANDLER_HAS_ERR 1
global interrupt_handler_%1

interrupt_handler_%1:
    push %1
    jmp interrupt_handler_generic
%endmacro

%assign i 0
;; TODO: comment/explain this malarky
%rep 256
    %if i == 8 || (i >= 10 && i <= 14) || i == 17
        GENERATE_HANDLER_HAS_ERR i
    %else
        GENERATE_HANDLER_NO_ERR i
    %endif
    %assign i i+1
%endrep
