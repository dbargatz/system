#include "frame.hpp"

interrupt_frame::interrupt_frame(logger& in_log, const void * in_frame_ptr) :
    _log(in_log), frame((struct _interrupt_frame *)in_frame_ptr) { }

void interrupt_frame::dump() {
    _log.panic("Interrupt : {#04X} ({})", frame->interrupt_number,
        frame->interrupt_number);
    _log.panic("Error Code: {#04X} ({})", frame->error_code, frame->error_code);
    _log.panic("Registers :");
    _log.panic("\trflags: {016X}", frame->rflags);
    _log.panic("\tcs:rip: {04X}:{016X}", frame->cs, frame->rip);
    _log.panic("\tss:rsp: {04X}:{016X}", frame->ss, frame->rsp);
    _log.panic("\trax: {016X}\trbx: {016X}\trcx: {016X}", frame->rax, 
        frame->rbx, frame->rcx);
    _log.panic("\trdx: {016X}\trdi: {016X}\trsi: {016X}", frame->rdx,
        frame->rdi, frame->rsi);
    _log.panic("\tr8 : {016X}\tr9 : {016X}\tr10: {016X}", frame->r8, frame->r9,
        frame->r10);
    _log.panic("\tr11: {016X}\tr12: {016X}\tr13: {016X}", frame->r11,
        frame->r12, frame->r13);
    _log.panic("\tr14: {016X}\tr15: {016X}", frame->r14, frame->r15);
}
