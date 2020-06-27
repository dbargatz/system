#include "frame.hpp"

interrupt_frame::interrupt_frame(logger& in_log, const void * in_frame_ptr) :
    _log(in_log), frame((struct _interrupt_frame *)in_frame_ptr) { }

void interrupt_frame::dump() {
    _log.debug("Registers:");
    _log.debug("\tcs : {#016X}\trip: {#016X}\tss : {#016X}\trsp: {#016X}",
               frame->cs, frame->rip, frame->ss, frame->rsp);
    _log.debug("\trax: {#016X}\trbx: {#016X}\trcx: {#016X}\trdx: {#016X}",
               frame->rax, frame->rbx, frame->rcx, frame->rdx);
    _log.debug("\trdi: {#016X}\trsi: {#016X}\tr8 : {#016X}\tr9 : {#016X}",
               frame->rdi, frame->rsi, frame->r8, frame->r9);
    _log.debug("\tr10: {#016X}\tr11: {#016X}\tr12: {#016X}\tr13: {#016X}",
               frame->r10, frame->r11, frame->r12, frame->r13);
    _log.debug("\tr14: {#016X}\tr15: {#016X}", frame->r14, frame->r15);
    _log.debug("\trflags: {#016X}", frame->rflags);
}
