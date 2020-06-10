#ifndef _PANIC_H
#define _PANIC_H

/**
 * @brief Data associated with a particular invocation of PANIC(). Includes 
 * filename and line number for invocation of PANIC(), as well as caller-
 * supplied message (if any).
 */
struct panic_data {
    const uint16_t instruction;  ///< Bytes of instruction that caused panic
    const char * filename;       ///< Source filename of PANIC() invocation
    const uint16_t lineNum;      ///< Line number of PANIC() invocation
    const char * msg;            ///< String message describing PANIC()
} __attribute__((packed));

/**
 * @brief Immediately transitions to ring 0, invokes the "ud2" interrupt handler
 * to output the source filename/line and cause of the PANIC().
 * 
 * @param msg the cause of/reason for the PANIC()
 */
#define PANIC(msg)                                         \
do {                                                       \
    asm volatile("ud2        # panic_data::instruction\n"  \
                 ".quad %c0  # panic_data::filename   \n"  \
                 ".word %c1  # panic_data::lineNum    \n"  \
                 ".quad %c2  # panic_data::msg        \n"  \
                 : : "i" (__FILE__), "i" (__LINE__),       \
                     "i" (msg));                           \
} while(0)

#endif // _PANIC_H