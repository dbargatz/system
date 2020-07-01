#ifndef _INTERRUPTS_TSS_HPP
#define _INTERRUPTS_TSS_HPP

#include "../std/logger.hpp"
#include "../std/stdint.h"
#include "gdt.hpp"

/**
 * Task State Segment implementation for x64 processors. Provides Interrupt
 * Stack Tables (ISTs) for known-good stacks when handling a PANIC/ASSERT or
 * unhandled interrupts.
 */
class tss {
public:
    tss(logger& in_log, gdt& in_gdt);

    void dump();

private:
    /**
     * Actual 64-bit Task State Segment structure used by processor.
     */
    struct _tss {
        uint32_t reserved1;       ///< Reserved; must be zero.
        uint32_t rsp0_low;        ///< Default stack; used when IST index is zero and privilege level changes to 0.
        uint32_t rsp0_high;       ///< Default stack; used when IST index is zero and privilege level changes to 0.
        uint32_t rsp1_low;        ///< Default stack; used when IST index is zero and privilege level changes to 1.
        uint32_t rsp1_high;       ///< Default stack; used when IST index is zero and privilege level changes to 1.
        uint32_t rsp2_low;        ///< Default stack; used when IST index is zero and privilege level changes to 2.
        uint32_t rsp2_high;       ///< Default stack; used when IST index is zero and privilege level changes to 2.
        uint64_t reserved2;       ///< Reserved; must be zero.
        uint32_t ist1_low;        ///< Interrupt Stack Table 1; used for PANIC(), ASSERT(), undefined opcode.
        uint32_t ist1_high;       ///< Interrupt Stack Table 1; used for PANIC(), ASSERT(), undefined opcode.
        uint32_t ist2_low;        ///< Interrupt Stack Table 2; used for unhandled interrupts vectors.
        uint32_t ist2_high;       ///< Interrupt Stack Table 2; used for unhandled interrupts vectors.
        uint32_t ist3_low;        ///< Interrupt Stack Table 3; unused.
        uint32_t ist3_high;       ///< Interrupt Stack Table 3; unused.
        uint32_t ist4_low;        ///< Interrupt Stack Table 4; unused.
        uint32_t ist4_high;       ///< Interrupt Stack Table 4; unused.
        uint32_t ist5_low;        ///< Interrupt Stack Table 5; unused.
        uint32_t ist5_high;       ///< Interrupt Stack Table 5; unused.
        uint32_t ist6_low;        ///< Interrupt Stack Table 6; unused.
        uint32_t ist6_high;       ///< Interrupt Stack Table 6; unused.
        uint32_t ist7_low;        ///< Interrupt Stack Table 7; unused.
        uint32_t ist7_high;       ///< Interrupt Stack Table 7; unused.
        uint64_t reserved3;       ///< Reserved; must be zero.
        uint16_t reserved4;       ///< Reserved; must be zero.
        uint16_t iopb_offset;     ///< I/O Port Bitmap; unused, must be sizeof(struct _tss).
    } __attribute__((packed));

    ///< Logger used by class.
    logger& _log;

    ///< Actual TSS for this core.
    struct _tss _our_tss;
};

#endif // _INTERRUPTS_TSS_HPP
