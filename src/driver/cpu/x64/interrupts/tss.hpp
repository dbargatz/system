#ifndef _INTERRUPTS_TSS_HPP
#define _INTERRUPTS_TSS_HPP

#include <cstdint>
#include "../../../../lib/libsystem/logger.hpp"
#include "gdt.hpp"

/**
 * Task State Segment implementation for x64 processors. Provides Interrupt
 * Stack Tables (ISTs) for known-good stacks when handling a PANIC/ASSERT or
 * unhandled interrupts.
 */
class tss {
public:
    tss(gdt& in_gdt);

    void dump(logging::logger& in_log);

private:
    /**
     * Actual 64-bit Task State Segment structure used by processor.
     */
    struct _tss {
        std::uint32_t reserved1;   ///< Reserved; must be zero.
        std::uint32_t rsp0_low;    ///< Default stack; used when IST index is zero and privilege level changes to 0.
        std::uint32_t rsp0_high;   ///< Default stack; used when IST index is zero and privilege level changes to 0.
        std::uint32_t rsp1_low;    ///< Default stack; used when IST index is zero and privilege level changes to 1.
        std::uint32_t rsp1_high;   ///< Default stack; used when IST index is zero and privilege level changes to 1.
        std::uint32_t rsp2_low;    ///< Default stack; used when IST index is zero and privilege level changes to 2.
        std::uint32_t rsp2_high;   ///< Default stack; used when IST index is zero and privilege level changes to 2.
        std::uint64_t reserved2;   ///< Reserved; must be zero.
        std::uint32_t ist1_low;    ///< Interrupt Stack Table 1; used for PANIC(), ASSERT(), undefined opcode.
        std::uint32_t ist1_high;   ///< Interrupt Stack Table 1; used for PANIC(), ASSERT(), undefined opcode.
        std::uint32_t ist2_low;    ///< Interrupt Stack Table 2; used for unhandled interrupts vectors.
        std::uint32_t ist2_high;   ///< Interrupt Stack Table 2; used for unhandled interrupts vectors.
        std::uint32_t ist3_low;    ///< Interrupt Stack Table 3; unused.
        std::uint32_t ist3_high;   ///< Interrupt Stack Table 3; unused.
        std::uint32_t ist4_low;    ///< Interrupt Stack Table 4; unused.
        std::uint32_t ist4_high;   ///< Interrupt Stack Table 4; unused.
        std::uint32_t ist5_low;    ///< Interrupt Stack Table 5; unused.
        std::uint32_t ist5_high;   ///< Interrupt Stack Table 5; unused.
        std::uint32_t ist6_low;    ///< Interrupt Stack Table 6; unused.
        std::uint32_t ist6_high;   ///< Interrupt Stack Table 6; unused.
        std::uint32_t ist7_low;    ///< Interrupt Stack Table 7; unused.
        std::uint32_t ist7_high;   ///< Interrupt Stack Table 7; unused.
        std::uint64_t reserved3;   ///< Reserved; must be zero.
        std::uint16_t reserved4;   ///< Reserved; must be zero.
        std::uint16_t iopb_offset; ///< I/O Port Bitmap; unused, must be sizeof(struct _tss).
    } __attribute__((packed));

    ///< Actual TSS for this core.
    struct _tss _our_tss;
};

#endif // _INTERRUPTS_TSS_HPP
