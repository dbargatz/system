#include <cstdlib>

[[noreturn]] void std::abort(void) {
    // Invoking the "brk" (software breakpoint) instruction on aarch64 causes a Breakpoint
    // Exception, which is handled by the platfom-specific interrupt handling system.
    asm volatile("brk 0xF2B6");

    // This will never be reached, but indicates to the compiler that this function never returns.
    while(true) {}
}
