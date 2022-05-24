#include <cstdint>
#include <cstdlib>
#include <memory_resource>
#include "../../../libs/libdevicetree/src/fdt.hpp"
#include "../../../libs/libdevicetree/src/properties/reg.hpp"

#include "console/console.hpp"
#include "memory/manager.hpp"
#include "platform.hpp"

// TODO: need structure to hold per-core information, like memory manager for each core

// This is defined in libcxx/new.cpp, and is how new/delete allocate and deallocate memory inside
// the core driver.
extern core::memory::memory_manager * _core_memory_manager;

// This is defined in libcxx/cassert.cpp and is used for logging during asserts.
extern core::console::console * _core_assert_log;

#define bail(msg) [] (auto exp) { assertm(false, msg); }

alignas(PAGE_SIZE_BYTES) static std::uint8_t buf[PAGE_SIZE_BYTES] = {0};

[[noreturn]] extern "C" void core_entry(std::uint64_t in_proc_id, const core::memory::physical_addr_t in_boot_info) {
    auto fdt = devicetree::fdt(in_boot_info);
    auto serial = fdt.find([] (devicetree::node& n) { 
        auto compatible = n.get_value<devicetree::properties::stringlist>("compatible");
        if(!compatible) { return false; }

        for(auto&& str : *compatible) {
            auto sv = std::string_view(str);
            if(0 == sv.compare("brcm,bcm2835-aux-uart")) {
                return true;
            }
        }
        return false;
    });
    assertm(serial, "no serial nodes present in devicetree");

    core::console::console log(core::console::level::Debug);
    _core_assert_log = &log;

    auto root = fdt.root().or_else(bail("root node not present in devicetree"));
    auto addr_cells = root->get_value<std::uint32_t>("#address-cells").value_or(0);
    auto size_cells = root->get_value<std::uint32_t>("#size-cells").value_or(0);
    assertm(addr_cells && size_cells, "invalid #address-cells and/or #size-cells");

    auto buf_resource = std::pmr::monotonic_buffer_resource(buf, sizeof(buf));
    auto mem_mgr = core::memory::memory_manager(&buf_resource);
    _core_memory_manager = &mem_mgr;

    log.info("Reserved Memory (FDT root):");
    for(auto&& range : fdt.reserved_memory()) {
        auto addr = (core::memory::physical_addr_t)devicetree::details::be_to_host(range->address);
        auto size = devicetree::details::be_to_host(range->size);
        log.info("  address: 0x{:X}", addr);
        log.info("  size: 0x{:X}", size);
        mem_mgr.register_pages(addr, size, core::memory::reservation_type::RESERVED_DEVICE);
    }

    auto memnode = fdt.get("/memory").or_else(bail("/memory node not present in devicetree"));
    auto memreg = memnode->get_reg_property(addr_cells, size_cells).or_else(bail("/memory node missing 'reg' property"));
    log.info("Memory Ranges:");
    for(auto&& reg : *memreg) {
        auto addr = (core::memory::physical_addr_t)reg.base;
        log.info("  base: {:X}", reg.base);
        log.info("  length: {}", reg.length);
        mem_mgr.register_pages(addr, reg.length, core::memory::reservation_type::UNALLOCATED);
    }

    auto reserved_mem = fdt.get("/reserved-memory").or_else(bail("/reserved-memory node not present in devicetree"));
    auto res_addr_cells = reserved_mem->get_value<std::uint32_t>("#address-cells").value_or(addr_cells);
    auto res_size_cells = reserved_mem->get_value<std::uint32_t>("#size-cells").value_or(size_cells);
    auto resmem_range = reserved_mem->get_ranges_property(addr_cells, size_cells, res_addr_cells, res_size_cells).or_else(bail("/reserved-memory node missing 'ranges' property"));
    log.info("Reserved Memory Mappings:");
    for(auto&& range : *resmem_range) {
        auto soc_bus_addr = (core::memory::physical_addr_t)range.parent_bus_address;
        log.info("  child bus addr : {:X}", range.child_bus_address);
        log.info("  parent bus addr: {:X}", range.parent_bus_address);
        log.info("  length: {}", range.length);
        mem_mgr.register_pages(soc_bus_addr, range.length, core::memory::reservation_type::RESERVED_DEVICE);
    }

    auto perm = get_permission_level();
    auto model = root->get_value<std::string_view>("model");
    log.info("Starting core driver for {} on processor {:X} at permission level {}", *model, in_proc_id, perm);
    log.info("{}", *memnode);
    log.info("{}", *serial);
    log.info("{}", mem_mgr);

    log.info("/ {");
    for(auto&& prop : root->properties()) {
        log.info("  {}", prop);
    }
    log.info("}");

    auto devtype_mem = fdt.find([&log] (devicetree::node& n) { 
        auto device_type = n.get_value<std::string_view>("device_type");
        if(!device_type || *device_type != std::string_view("memory")) { return false; }

        log.info("{}", n);
        return false;
    });

    log.unicode_test(core::console::level::Debug);

    // TODO: Initialize core state manager
    // TODO: Initialize interrupt manager
    // TODO: Initialize full memory manager

    log.error("Aborting!");
    std::abort();
}
