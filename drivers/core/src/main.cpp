#include <cstdint>

#include "platform.hpp"

extern "C" void core_entry(std::uint64_t in_proc_id) {
    core::console::uart log;
    log.write("Hello, world!\n");
}
