#include <array>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include "../../../libs/libdevicetree/src/fdt.hpp"

#include "console/console.hpp"
#include "memory/manager.hpp"
#include "platform.hpp"


// TODO: need structure to hold per-core information, like memory manager for each core

// This is defined in libcxx/new.cpp, and is how new/delete allocate and deallocate memory inside
// the core driver.
extern core::memory::memory_manager * _core_memory_manager;

[[noreturn]] extern "C" void core_entry(std::uint64_t in_proc_id, const core::memory::physical_addr_t in_boot_info) {
    core::console::console log(core::console::level::Debug);
    log.puts("Console init\n");

    auto mem_mgr = core::memory::memory_manager();
    _core_memory_manager = &mem_mgr;

    log.info("Starting core driver for {} on processor {:X}", PLATFORM_NAME, in_proc_id);
    log.info("{}", mem_mgr);
    auto fdt = devicetree::fdt::parse(in_boot_info);
    log.info("{}", fdt);

    auto memnode = fdt.find("/memory");
    log.info("{}", *memnode);
    log.unicode_test(core::console::level::Debug);

    std::array<std::uint64_t, 15> arr;
    for(auto i = 0; i < 15; i++) { arr[i] = 1 << i; }
    log.info("std::array test:");
    for(auto&& element : arr) { log.info("  Element: 0x{:X}", element); }

    auto vec = std::vector<std::uint64_t>(32, 0x1122334455667788);
    log.info("std::vector test (size={}, cap={}):", vec.size(), vec.capacity());
    for(auto i = 0; i < vec.size(); i++) {
        log.info("  Element: 0x{:X}", vec[i]);
    }

    log.info("{}", mem_mgr);

    // TODO: Initialize core state manager
    // TODO: Initialize interrupt manager
    // TODO: Initialize full memory manager

    log.error("Aborting!");
    std::abort();
}
