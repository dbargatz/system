#include <cstdint>
#include <cstdlib>
#include "../../../libs/libdevicetree/src/fdt.hpp"

#include "console/console.hpp"
#include "memory/manager.hpp"
#include "platform.hpp"

// TODO: need structure to hold per-core information, like memory manager for each core

// This is defined in libcxx/new.cpp, and is how new/delete allocate and deallocate memory inside
// the core driver.
extern core::memory::memory_manager * _core_memory_manager;

// This is defined in libcxx/cassert.cpp and is used for logging during asserts.
extern core::console::console * _core_assert_log;

[[noreturn]] extern "C" void core_entry(std::uint64_t in_proc_id, const core::memory::physical_addr_t in_boot_info) {
    core::console::console log(core::console::level::Debug);
    _core_assert_log = &log;

    auto fdt = devicetree::fdt(in_boot_info);
    devicetree::node root;
    assert(fdt.find("/", &root));
    devicetree::node memnode;
    assert(fdt.find("/memory", &memnode));

    auto mem_mgr = core::memory::memory_manager();
    _core_memory_manager = &mem_mgr;

    auto perm = get_permission_level();
    log.info("Starting core driver for {} on processor {:X} at permission level {}", PLATFORM_NAME, in_proc_id, perm);
    log.info("{}", memnode);

    log.info("{}", mem_mgr);

    log.info("/ {");
    for(auto&& prop : root.properties()) {
        log.info("  {}", prop);
    }
    log.info("}");

    log.info("{}", mem_mgr);

    log.unicode_test(core::console::level::Debug);

    // TODO: Initialize core state manager
    // TODO: Initialize interrupt manager
    // TODO: Initialize full memory manager

    log.error("Aborting!");
    std::abort();
}
