#include <cstdint>
#include <cstdlib>

#include "console/console.hpp"

extern "C" void core_entry(std::uint64_t in_proc_id) {
    core::console::console log;
    log.debug("Hello, world!");
    log.error("Aborting!");
    std::abort();
}
