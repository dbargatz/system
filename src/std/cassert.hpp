#ifndef _STD_CASSERT_HPP
#define _STD_CASSERT_HPP

#ifdef NDEBUG
#define assert(cond) ((void)0)
#else // NDEBUG

#define STRINGIFY_INTERNAL(x) #x
#define STRINGIFY(x) STRINGIFY_INTERNAL(x)

// TODO: this implementation is implicitly reliant on the PANIC() definition,
//       as well as nastily hardcoded. Fix this.
#define assert(cond) \
    if(!(cond)) {                                                     \
        asm volatile("ud2       # panic_data::instruction\n"        \
                    ".word %c0  # panic_data::type       \n"        \
                    ".quad %c1  # panic_data::filename   \n"        \
                    ".word %c2  # panic_data::lineNum    \n"        \
                    ".quad %c3  # panic_data::msg        \n"        \
                    : /* No outputs */                              \
                    : "i" (0xF2B5), "i" (__FILE__),         \
                    "i" (__LINE__), "i" ("" __FILE__ ":" STRINGIFY(__LINE__) ": " STRINGIFY(__func__) ": Assertion '" #cond "' failed.") \
                    : /* No clobbers */);                           \
    }
#endif // NDEBUG

#endif // _STD_CASSERT_HPP
