#include "tss.hpp"

extern uint64_t ist1_stack_top;
extern uint64_t ist2_stack_top;

tss::tss(logger& in_log, gdt& in_gdt) : _log(in_log) {
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
    uint64_t rsp;
    asm volatile("movq %%rsp, %0" : "=m"(rsp));
    _our_tss.rsp0_low  = (uint32_t)((rsp & 0x00000000FFFFFFFF) >> 0);
    _our_tss.rsp0_high = (uint32_t)((rsp & 0xFFFFFFFF00000000) >> 32);
    _our_tss.rsp1_low  = (uint32_t)0;
    _our_tss.rsp1_high = (uint32_t)0;
    _our_tss.rsp2_low  = (uint32_t)0;
    _our_tss.rsp2_high = (uint32_t)0;

    // Load the ISTs into their respective positions, and zero-out the rest.
    uint64_t ist1 = (uint64_t)&ist1_stack_top;
    uint64_t ist2 = (uint64_t)&ist2_stack_top;
    _our_tss.ist1_low  = (uint32_t)((ist1 & 0x00000000FFFFFFFF) >> 0);
    _our_tss.ist1_high = (uint32_t)((ist1 & 0xFFFFFFFF00000000) >> 32);
    _our_tss.ist2_low  = (uint32_t)((ist2 & 0x00000000FFFFFFFF) >> 0);
    _our_tss.ist2_high = (uint32_t)((ist2 & 0xFFFFFFFF00000000) >> 32);
    _our_tss.ist3_low  = (uint32_t)0;
    _our_tss.ist3_high = (uint32_t)0;
    _our_tss.ist4_low  = (uint32_t)0;
    _our_tss.ist4_high = (uint32_t)0;
    _our_tss.ist5_low  = (uint32_t)0;
    _our_tss.ist5_high = (uint32_t)0;
    _our_tss.ist6_low  = (uint32_t)0;
    _our_tss.ist6_high = (uint32_t)0;
    _our_tss.ist7_low  = (uint32_t)0;
    _our_tss.ist7_high = (uint32_t)0;

    // Load the I/O Port Bitmap with the size of the TSS structure, effectively
    // disabling it.
    // TODO: verify this with the Intel manuals.
    _our_tss.iopb_offset = (uint16_t)sizeof(_our_tss);

    // Install the descriptor for the TSS into the GDT. Note that this is a
    // 64-bit TSS, so there are two consecutive 8-byte entries in the GDT; the
    // second entry is bits 32-63 of the TSS base address (if applicable).
    in_gdt.install(0x28, &_our_tss, sizeof(_our_tss), 0x89, 0x00);
    in_gdt.install(0x30, 0, 0, 0, 0);

    // Load the TSS into the core.
    asm volatile("movq $0x28, %%rax; ltr %%ax": : : "rax");
}

void tss::dump() {
    _log.debug("TSS           : {#016X}", (uint64_t)&_our_tss);
    _log.debug("\tRSP0         : {#016X}", _our_tss.rsp0_low + (((uint64_t)_our_tss.rsp0_high << 32)));
    _log.debug("\tRSP1         : {#016X}", _our_tss.rsp1_low + (((uint64_t)_our_tss.rsp1_high << 32)));
    _log.debug("\tRSP2         : {#016X}", _our_tss.rsp2_low + (((uint64_t)_our_tss.rsp2_high << 32)));
    _log.debug("\tIST1         : {#016X}", _our_tss.ist1_low + (((uint64_t)_our_tss.ist1_high << 32)));
    _log.debug("\tIST2         : {#016X}", _our_tss.ist2_low + (((uint64_t)_our_tss.ist2_high << 32)));
    _log.debug("\tIST3         : {#016X}", _our_tss.ist3_low + (((uint64_t)_our_tss.ist3_high << 32)));
    _log.debug("\tIST4         : {#016X}", _our_tss.ist4_low + (((uint64_t)_our_tss.ist4_high << 32)));
    _log.debug("\tIST5         : {#016X}", _our_tss.ist5_low + (((uint64_t)_our_tss.ist5_high << 32)));
    _log.debug("\tIST6         : {#016X}", _our_tss.ist6_low + (((uint64_t)_our_tss.ist6_high << 32)));
    _log.debug("\tIST7         : {#016X}", _our_tss.ist7_low + (((uint64_t)_our_tss.ist7_high << 32)));
    _log.debug("\tIOPB Offset  : {#04X}", _our_tss.iopb_offset);
}
