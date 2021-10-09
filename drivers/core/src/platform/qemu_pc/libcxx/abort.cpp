#include <cstdlib>

// #define STRINGIFY_INTERNAL(x) #x
// #define STRINGIFY(x) STRINGIFY_INTERNAL(x)
// #define _ASSERT_MSG(cond) __FILE__ ":" STRINGIFY(__LINE__) ": Assertion '" #cond "' failed."

void std::abort(void) {
    asm volatile("ud2");
    // asm volatile("ud2        # panic_data::instruction\n"        \
    //              ".word %c0  # panic_data::type       \n"        \
    //              ".quad %c1  # panic_data::filename   \n"        \
    //              ".word %c2  # panic_data::lineNum    \n"        \
    //              ".quad %c3  # panic_data::msg        \n"        \
    //              : /* No outputs */                              \
    //              : "i" (0xF2B6), "i" (__FILE__), "i" (__LINE__), \
    //              : "i" ("abort called")                          \
    //              : /* No clobbers */);
}
