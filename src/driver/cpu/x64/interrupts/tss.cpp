#include "tss.hpp"

extern std::uint64_t ist1_stack_top;
extern std::uint64_t ist2_stack_top;

tss::tss(gdt& in_gdt) {
    // Zero out the reserved fields.
    _our_tss.reserved1 = (std::uint32_t)0;
    _our_tss.reserved2 = (std::uint64_t)0;
    _our_tss.reserved3 = (std::uint64_t)0;
    _our_tss.reserved4 = (std::uint16_t)0;

    // Load the default stack into rsp0 (for use when the privilege level
    // changes from 3/2/1 to 0); zero out the other default stacks, since
    // privilege levels 2/1 should never be used.
    // TODO: This method of saving RSP takes the current call chain into
    //       account; however, we can't use stack_top from start.asm because
    //       of local variables in core_entry(). Is now the time to allocate a
    //       new default stack and switch to it?
    std::uint64_t rsp;
    asm volatile("movq %%rsp, %0" : "=m"(rsp));
    _our_tss.rsp0_low  = (std::uint32_t)((rsp & 0x00000000FFFFFFFF) >> 0);
    _our_tss.rsp0_high = (std::uint32_t)((rsp & 0xFFFFFFFF00000000) >> 32);
    _our_tss.rsp1_low  = (std::uint32_t)0;
    _our_tss.rsp1_high = (std::uint32_t)0;
    _our_tss.rsp2_low  = (std::uint32_t)0;
    _our_tss.rsp2_high = (std::uint32_t)0;

    // Load the ISTs into their respective positions, and zero-out the rest.
    std::uint64_t ist1 = (std::uint64_t)&ist1_stack_top;
    std::uint64_t ist2 = (std::uint64_t)&ist2_stack_top;
    _our_tss.ist1_low  = (std::uint32_t)((ist1 & 0x00000000FFFFFFFF) >> 0);
    _our_tss.ist1_high = (std::uint32_t)((ist1 & 0xFFFFFFFF00000000) >> 32);
    _our_tss.ist2_low  = (std::uint32_t)((ist2 & 0x00000000FFFFFFFF) >> 0);
    _our_tss.ist2_high = (std::uint32_t)((ist2 & 0xFFFFFFFF00000000) >> 32);
    _our_tss.ist3_low  = (std::uint32_t)0;
    _our_tss.ist3_high = (std::uint32_t)0;
    _our_tss.ist4_low  = (std::uint32_t)0;
    _our_tss.ist4_high = (std::uint32_t)0;
    _our_tss.ist5_low  = (std::uint32_t)0;
    _our_tss.ist5_high = (std::uint32_t)0;
    _our_tss.ist6_low  = (std::uint32_t)0;
    _our_tss.ist6_high = (std::uint32_t)0;
    _our_tss.ist7_low  = (std::uint32_t)0;
    _our_tss.ist7_high = (std::uint32_t)0;

    // Load the I/O Port Bitmap with the size of the TSS structure, effectively
    // disabling it. Per Intel SDM, Volume 1, Section 19.5.2: "If the I/O bit
    // map base address is greater than or equal to the TSS segment limit,
    // there is no I/O permission map, and all I/O instructions generate
    // exceptions when the CPL is greater than the current IOPL." Below, we
    // set the segment limit for the TSS descriptor as the size of the TSS,
    // meaning all I/O ports are inaccessible from user-mode (so long as the
    // IOPL is less than the current privilege level).
    _our_tss.iopb_offset = (std::uint16_t)sizeof(_our_tss);

    // Install the descriptor for the TSS into the GDT. Note that this is a
    // 64-bit TSS, so there are two consecutive 8-byte entries in the GDT; the
    // second entry is bits 32-63 of the TSS base address (if applicable). Note
    // the segment limit; setting it to the size of the TSS makes all I/O ports
    // inaccessible from user-mode. See above for more detail.
    in_gdt.install(0x30, &_our_tss, sizeof(_our_tss), 0x89, 0x00);
    in_gdt.install(0x38, 0, 0, 0, 0);

    // Load the TSS into the core.
    asm volatile("movq $0x30, %%rax; ltr %%ax": : : "rax");
}

void tss::dump(logging::logger& in_log) {
    in_log.debug(u8"TSS: {#016X}", (std::uint64_t)&_our_tss);
    in_log.debug(u8"\tRSP0       : {#016X}", _our_tss.rsp0_low + (((std::uint64_t)_our_tss.rsp0_high << 32)));
    in_log.debug(u8"\tRSP1       : {#016X}", _our_tss.rsp1_low + (((std::uint64_t)_our_tss.rsp1_high << 32)));
    in_log.debug(u8"\tRSP2       : {#016X}", _our_tss.rsp2_low + (((std::uint64_t)_our_tss.rsp2_high << 32)));
    in_log.debug(u8"\tIST1       : {#016X}", _our_tss.ist1_low + (((std::uint64_t)_our_tss.ist1_high << 32)));
    in_log.debug(u8"\tIST2       : {#016X}", _our_tss.ist2_low + (((std::uint64_t)_our_tss.ist2_high << 32)));
    in_log.debug(u8"\tIST3       : {#016X}", _our_tss.ist3_low + (((std::uint64_t)_our_tss.ist3_high << 32)));
    in_log.debug(u8"\tIST4       : {#016X}", _our_tss.ist4_low + (((std::uint64_t)_our_tss.ist4_high << 32)));
    in_log.debug(u8"\tIST5       : {#016X}", _our_tss.ist5_low + (((std::uint64_t)_our_tss.ist5_high << 32)));
    in_log.debug(u8"\tIST6       : {#016X}", _our_tss.ist6_low + (((std::uint64_t)_our_tss.ist6_high << 32)));
    in_log.debug(u8"\tIST7       : {#016X}", _our_tss.ist7_low + (((std::uint64_t)_our_tss.ist7_high << 32)));
    in_log.debug(u8"\tIOPB Offset: {#04X}", _our_tss.iopb_offset);
}
