#include <cstdint>
#include <cstdlib>

#include "console/console.hpp"
#include "memory/manager.hpp"
#include "platform.hpp"

alignas(PAGE_SIZE_BYTES) static std::uint8_t core_heap_bytes[PAGE_SIZE_BYTES * 16] = {0};

[[noreturn]] extern "C" void core_entry(std::uint64_t in_proc_id, const void* in_boot_info) {
    core::console::console log(core::console::level::Debug);
    auto memory = core::memory::memory_manager(log, core_heap_bytes, 16);

    log.info("Starting core driver for {} on processor {:X}", PLATFORM_NAME, in_proc_id);
    log.info("  Boot Info: {:016X}", in_boot_info);
    log.unicode_test(core::console::level::Debug);

    // TODO: Initialize core state manager
    // TODO: Initialize interrupt manager
    // TODO: Initialize full memory manager

    log.error("Aborting!");
    std::abort();
}
