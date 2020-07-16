#include "frame.hpp"

interrupt_frame::interrupt_frame(const void * in_frame_ptr) :
    frame((struct _interrupt_frame *)in_frame_ptr) { }

void interrupt_frame::dump(logger& in_log) {
    in_log.panic("Interrupt : {#04X} ({})", frame->interrupt_number,
        frame->interrupt_number);
    in_log.panic("Error Code: {#04X} ({})", frame->error_code, frame->error_code);
    in_log.panic("Registers :");
    in_log.panic("\trflags: {016X}", frame->rflags);
    in_log.panic("\tcs:rip: {04X}:{016X}", frame->cs, frame->rip);
    in_log.panic("\tss:rsp: {04X}:{016X}", frame->ss, frame->rsp);
    in_log.panic("\trax: {016X}\trbx: {016X}\trcx: {016X}", frame->rax, 
        frame->rbx, frame->rcx);
    in_log.panic("\trdx: {016X}\trdi: {016X}\trsi: {016X}", frame->rdx,
        frame->rdi, frame->rsi);
    in_log.panic("\tr8 : {016X}\tr9 : {016X}\tr10: {016X}", frame->r8, frame->r9,
        frame->r10);
    in_log.panic("\tr11: {016X}\tr12: {016X}\tr13: {016X}", frame->r11,
        frame->r12, frame->r13);
    in_log.panic("\tr14: {016X}\tr15: {016X}", frame->r14, frame->r15);
}
