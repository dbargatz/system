#include <cstdint>
#include <cstdlib>

#include "console/console.hpp"
#include "platform.hpp"

[[noreturn]] extern "C" void core_entry(std::uint64_t in_proc_id, const void* in_boot_info) {
    core::console::console log(core::console::level::Info);

    log.info("Starting core driver for {} on processor {:X}", PLATFORM_NAME, in_proc_id);
    log.info("  Boot Info: {:016X}", in_boot_info);
    log.unicode_test(core::console::level::Debug);

    // TODO: Initialize core state manager
    // TODO: Initialize interrupt manager
    // TODO: Initialize memory manager

    log.error("Aborting!");
    std::abort();
}
