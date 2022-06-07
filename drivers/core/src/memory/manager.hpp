#ifndef _CORE_MEMORY_MANAGER_HPP
#define _CORE_MEMORY_MANAGER_HPP

#include <cassert>
#include <cstdint>
#include <format>
#include <memory_resource>
#include "map.hpp"

namespace core::memory {

class memory_manager {
private:
    std::pmr::monotonic_buffer_resource _assert_heap;
    std::pmr::monotonic_buffer_resource _core_heap;
    map _memory_map;

public:
    memory_manager();
    physical_addr_t core_allocate(const std::size_t in_size, const std::align_val_t in_alignment = (const std::align_val_t)16);
    void core_deallocate(const physical_addr_t in_addr);

    void register_range(const physical_addr_t in_start, const std::size_t in_size, const reservation_type in_type);

    auto format() const {
        auto str = std::format(
            "Memory Manager at 0x{:X}:\n"
            "  Memory Map    : {} bytes\n"
            "  {}\n",
            (physical_addr_t)this,
            sizeof(_memory_map),
            _memory_map
        );

        return str;
    }
};

}; // namespace core::memory

template <>
struct std::formatter<core::memory::memory_manager> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const core::memory::memory_manager& in_arg) {
        return in_arg.format();
    }
};

#endif // _CORE_MEMORY_MANAGER_HPP