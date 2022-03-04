#include <cstdint>
#include <cstdlib>

#include "console/console.hpp"
#include "memory/manager.hpp"
#include "platform.hpp"

// TODO: need structure to hold per-core information, like memory manager for each core

// This is defined in libcxx/new.cpp, and is how new/delete allocate and deallocate memory inside
// the core driver.
extern core::memory::memory_manager * _core_memory_manager;

[[noreturn]] extern "C" void core_entry(std::uint64_t in_proc_id, const void* in_boot_info) {
    core::console::console log(core::console::level::Debug);
    auto mem_mgr = core::memory::memory_manager();
    _core_memory_manager = &mem_mgr;

    log.info("Starting core driver for {} on processor {:X}", PLATFORM_NAME, in_proc_id);
    log.info("  Boot Info: {:016X}", in_boot_info);
    log.unicode_test(core::console::level::Debug);

    // TODO: Initialize core state manager
    // TODO: Initialize interrupt manager
    // TODO: Initialize full memory manager

    log.error("Aborting!");
    std::abort();
}
