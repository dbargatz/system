#include <cstdint>
#include <cstdlib>

#include "console/console.hpp"
#include "platform.hpp"

[[noreturn]] extern "C" void core_entry(std::uint64_t in_proc_id, const void* in_boot_info) {
    core::console::console log;
    log.info("Starting core driver for {} on processor {:X}", PLATFORM_NAME, in_proc_id);
    log.info("  Boot Info: {:016X}", in_boot_info);
    log.error("Aborting!");
    std::abort();
}
