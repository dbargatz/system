#include "frame.hpp"

interrupt_frame::interrupt_frame(logger& in_log, const void * in_frame_ptr) :
    _log(in_log), frame((struct _interrupt_frame *)in_frame_ptr) { }

void interrupt_frame::dump(logger::level in_level, uint8_t in_indent) {
    _log.write(in_level, "Registers:\n");
    _log.write(in_level, "\tcs : {#016X}\trip: {#016X}\tss : {#016X}\trsp: {#016X}\n",
               frame->cs, frame->rip, frame->ss, frame->rsp);
    _log.write(in_level, "\trax: {#016X}\trbx: {#016X}\trcx: {#016X}\trdx: {#016X}\n",
               frame->rax, frame->rbx, frame->rcx, frame->rdx);
    _log.write(in_level, "\trdi: {#016X}\trsi: {#016X}\tr8 : {#016X}\tr9 : {#016X}\n",
               frame->rdi, frame->rsi, frame->r8, frame->r9);
    _log.write(in_level, "\tr10: {#016X}\tr11: {#016X}\tr12: {#016X}\tr13: {#016X}\n",
               frame->r10, frame->r11, frame->r12, frame->r13);
    _log.write(in_level, "\tr14: {#016X}\tr15: {#016X}\n", frame->r14, frame->r15);
    _log.write(in_level, "\trflags: {#016X}\n", frame->rflags);
}
