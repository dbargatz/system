#ifndef _INTERRUPTS_TSS_HPP
#define _INTERRUPTS_TSS_HPP

#include "../std/logger.hpp"
#include "../std/stdint.h"

/**
 * Task State Segment implementation for x64 processors. Provides Interrupt
 * Stack Tables (ISTs) for known-good stacks when handling a PANIC/ASSERT or
 * unhandled interrupts.
 */
class tss {
public:
    tss(logger& in_log);

    void dump();

private:
    /**
     * Actual 64-bit Task State Segment structure used by processor.
     */
    struct _tss {
        uint32_t reserved1;       ///< Reserved; must be zero.
        uint64_t rsp0;            ///< Default stack; used when IST index is zero and privilege level changes to 0.
        uint64_t rsp1;            ///< Default stack; used when IST index is zero and privilege level changes to 1.
        uint64_t rsp2;            ///< Default stack; used when IST index is zero and privilege level changes to 2.
        uint64_t reserved2;       ///< Reserved; must be zero.
        uint64_t ist1;            ///< Interrupt Stack Table 1; used for PANIC(), ASSERT(), undefined opcode.
        uint64_t ist2;            ///< Interrupt Stack Table 2; used for unhandled interrupts vectors.
        uint64_t ist3;            ///< Interrupt Stack Table 3; unused.
        uint64_t ist4;            ///< Interrupt Stack Table 4; unused.
        uint64_t ist5;            ///< Interrupt Stack Table 5; unused.
        uint64_t ist6;            ///< Interrupt Stack Table 6; unused.
        uint64_t ist7;            ///< Interrupt Stack Table 7; unused.
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
