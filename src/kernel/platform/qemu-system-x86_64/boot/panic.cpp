#include "panic.h"
#include "logger.hpp"

extern "C" void panic(const char * in_str) {
    kernel::platform::x86_64::logger log;

    log.panic(in_str);

    // TODO: dump registers/state

    while(1) {
        // Loop forever.
    }
}