#include "map.hpp"

core::memory::map::map(std::pmr::memory_resource * in_backing_memory) {
    auto listmem = in_backing_memory->allocate(sizeof(*_ranges));
    auto alloc = std::pmr::polymorphic_allocator<struct details::address_range>(in_backing_memory);
    _ranges = new(listmem) std::pmr::list<struct details::address_range>(alloc);
}

void core::memory::map::register_range(const core::memory::reservation_type in_type, const physical_addr_t in_start_addr, const std::size_t in_size_bytes) {
    auto new_range = core::memory::details::address_range {
        .start_addr = in_start_addr,
        .length_bytes = in_size_bytes,
        .type = in_type,
    };

    for(auto it = _ranges->begin(); it != _ranges->end();) {
        auto existing_range = *it;
        if(new_range.start_addr == existing_range.start_addr) {
            if(new_range.type <= existing_range.type) {
                new_range.start_addr += existing_range.length_bytes;
                new_range.length_bytes -= existing_range.length_bytes;
                continue;
            }

            _ranges->insert(it, new_range);
            existing_range.start_addr = new_range.start_addr + new_range.length_bytes;
            existing_range.length_bytes -= new_range.length_bytes;
            _ranges->insert(it, existing_range);
            _ranges->erase(it);
            return;
        }
        ++it;
    }

    _ranges->push_back(new_range);
}