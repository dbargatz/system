#include <cstdint>

#include "console/uart.hpp"

extern "C" void core_entry(std::uint64_t in_proc_id) {
    core::arm64::console::uart log;
    log.write("Hello, world!\n");
}
