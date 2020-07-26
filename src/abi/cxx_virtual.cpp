#include "cxxabi.h"

extern "C" void __cxa_pure_virtual() {
    // TODO: convert this to use PANIC() (or equivalent) once that's in user-mode.

    // Halt should cause the program to crash. Loop so the compiler understands
    // this will never return.
    while(true) {
        asm("hlt");
    }
}
