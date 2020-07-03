/**
 * @file
 * @brief Contains invalid opcode-based implementation of PANIC().
 */
#ifndef _STD_PANIC_H
#define _STD_PANIC_H

#include "stdint.h"

///< Encoding of the "ud2" Undefined Instruction. Used to trigger a PANIC().
constexpr static const uint16_t UD2_ENCODING = 0x0F0B;

/**
 * Different sub-types of PANIC(), which affect the way the output is formatted.
 * Also serves to validate an invalid opcode exception was triggered by an
 * actual PANIC() invocation, thus the random values.
 */
enum class panic_type : uint16_t {
    GENERIC       = 0xF2B4,    ///< Cause not specified by other types
    ASSERT_FAILED = 0xF2B5,    ///< PANIC() caused by failed ASSERT()
};

/**
 * Data associated with a particular invocation of PANIC(). Includes
 * filename and line number for invocation of PANIC(), as well as caller-
 * supplied message (if any).
 */
struct panic_data {
    const uint16_t instruction;  ///< Bytes of instruction that caused panic
    const panic_type type;       ///< Validates as actual PANIC() and its cause
    const char * filename;       ///< Source filename of PANIC() invocation
    const uint16_t lineNum;      ///< Line number of PANIC() invocation
    const char * msg;            ///< String message describing PANIC()
} __attribute__((packed));

/**
 * Immediately transitions to ring 0, invokes the "ud2" interrupt handler to
 * output the source filename/line and cause of the PANIC().
 * 
 * @param msg description of the cause of/reason for the PANIC()
 * @param type panic_type enum value to specialize PANIC() message formatting
 */
#define PANIC_TYPED(msg, type)                             \
do {                                                       \
    asm volatile("ud2        # panic_data::instruction\n"  \
                 ".word %c0  # panic_data::type       \n"  \
                 ".quad %c1  # panic_data::filename   \n"  \
                 ".word %c2  # panic_data::lineNum    \n"  \
                 ".quad %c3  # panic_data::msg        \n"  \
                 : /* No outputs */                        \
                 : "i" ((uint16_t)type), "i" (__FILE__),   \
                   "i" (__LINE__), "i" (msg)               \
                 : /* No clobbers */);                     \
} while(0)

/**
 * Immediately transitions to ring 0, invokes the "ud2" interrupt handler to
 * output the source filename/line and cause of the PANIC() with generic type.
 * 
 * @param msg description of the cause of/reason for the PANIC()
 */
#define PANIC(msg) PANIC_TYPED(msg, panic_type::GENERIC)

#endif // _STD_PANIC_H
