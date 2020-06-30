#include "tss.hpp"

extern uint64_t tss_space;
extern uint64_t ist1_stack_bottom;
extern uint64_t ist2_stack_bottom;

tss::tss(logger& in_log) : _log(in_log) {
    _core_tss = (struct _tss *)&tss_space;

    // Zero out the reserved fields.
    _core_tss->reserved1 = (uint32_t)0;
    _core_tss->reserved2 = (uint64_t)0;
    _core_tss->reserved3 = (uint64_t)0;
    _core_tss->reserved4 = (uint16_t)0;

    // Load the ISTs into their respective positions, and zero-out the rest.
    _core_tss->ist1 = (uint64_t)&ist1_stack_bottom;
    _core_tss->ist2 = (uint64_t)&ist2_stack_bottom;
    _core_tss->ist3 = (uint64_t)0;
    _core_tss->ist4 = (uint64_t)0;
    _core_tss->ist5 = (uint64_t)0;
    _core_tss->ist6 = (uint64_t)0;
    _core_tss->ist7 = (uint64_t)0;

    // Load the I/O Port Bitmap with the size of the TSS structure, effectively
    // disabling it.
    // TODO: verify this with the Intel manuals.
    _core_tss->iopb_offset = (uint16_t)sizeof(*_core_tss);

    _log.debug("TSS Space    : {#016X}", (uint64_t)_core_tss);
    _log.debug("IST1         : {#016X}", _core_tss->ist1);
    _log.debug("IST2         : {#016X}", _core_tss->ist2);
    _log.debug("IOPB Offset  : {#016X}", _core_tss->iopb_offset);
}
