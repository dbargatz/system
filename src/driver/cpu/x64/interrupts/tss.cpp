#include "tss.hpp"

extern uint64_t ist1_stack_top;
extern uint64_t ist2_stack_top;

tss::tss(logger& in_log) : _log(in_log) {
    // Zero out the reserved fields.
    _our_tss.reserved1 = (uint32_t)0;
    _our_tss.reserved2 = (uint64_t)0;
    _our_tss.reserved3 = (uint64_t)0;
    _our_tss.reserved4 = (uint16_t)0;

    // Load the default stack into rsp0 (for use when the privilege level
    // changes from 3/2/1 to 0); zero out the other default stacks, since
    // privilege levels 2/1 should never be used.
    // TODO: This method of saving RSP takes the current call chain into
    //       account; however, we can't use stack_top from start.asm because
    //       of local variables in kmain(). Is now the time to allocate a new
    //       default stack and switch to it?
    asm volatile("movq %%rsp, %0" : "=m"(_our_tss.rsp0));
    _our_tss.rsp1 = (uint64_t)0;
    _our_tss.rsp2 = (uint64_t)0;

    // Load the ISTs into their respective positions, and zero-out the rest.
    _our_tss.ist1 = (uint64_t)&ist1_stack_top;
    _our_tss.ist2 = (uint64_t)&ist2_stack_top;
    _our_tss.ist3 = (uint64_t)0;
    _our_tss.ist4 = (uint64_t)0;
    _our_tss.ist5 = (uint64_t)0;
    _our_tss.ist6 = (uint64_t)0;
    _our_tss.ist7 = (uint64_t)0;

    // Load the I/O Port Bitmap with the size of the TSS structure, effectively
    // disabling it.
    // TODO: verify this with the Intel manuals.
    _our_tss.iopb_offset = (uint16_t)sizeof(_our_tss);

    // Load the TSS into the core.
    asm volatile("movq $0x28, %%rax; ltr %%ax": : : "rax");
}

void tss::dump() {
    _log.debug("TSS           : {#016X}", (uint64_t)&_our_tss);
    _log.debug("\tRSP0         : {#016X}", _our_tss.rsp0);
    _log.debug("\tRSP1         : {#016X}", _our_tss.rsp1);
    _log.debug("\tRSP2         : {#016X}", _our_tss.rsp2);
    _log.debug("\tIST1         : {#016X}", _our_tss.ist1);
    _log.debug("\tIST2         : {#016X}", _our_tss.ist2);
    _log.debug("\tIST3         : {#016X}", _our_tss.ist3);
    _log.debug("\tIST4         : {#016X}", _our_tss.ist4);
    _log.debug("\tIST5         : {#016X}", _our_tss.ist5);
    _log.debug("\tIST6         : {#016X}", _our_tss.ist6);
    _log.debug("\tIST7         : {#016X}", _our_tss.ist7);
    _log.debug("\tIOPB Offset  : {#04X}", _our_tss.iopb_offset);
}
