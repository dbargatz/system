#include <cstdlib>

[[noreturn]] void std::abort(void) {
    // Invoking the "ud2" (undefined) instruction on x86_64 causes an Invalid Opcode exception,
    // which is handled by the platform-specific interrupt handling system.
    asm volatile("ud2");

    // This will never be reached, but indicates to the compiler that this function never returns.
    while(true) {}
}
