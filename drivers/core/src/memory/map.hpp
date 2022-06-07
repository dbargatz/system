#ifndef _CORE_MEMORY_MAP_HPP
#define _CORE_MEMORY_MAP_HPP

#include <cstddef>
#include <cstdint>
#include <list>
#include <memory_resource>
#include "types.hpp"

namespace core::memory {

enum reservation_type {
    UNINITIALIZED = 0,
    UNALLOCATED = 1,
    ALLOCATED = 2,
    RESERVED_CORE_DRIVER = 3,
    RESERVED_DEVICE = 4,
};

namespace details {

struct address_range {
    physical_addr_t start_addr;
    std::size_t length_bytes;
    reservation_type type;
} __attribute__((packed));

}; // namespace details

class map {
private:
    std::pmr::list<struct details::address_range> * _ranges;

public:
    map() = delete;
    map(std::pmr::memory_resource * in_backing_memory);

    auto format() const {
        auto str = std::string("Memory map:\n");
        if(_ranges->empty()) { str.append("    Empty\n"); return str; }

        for(auto&& range : *_ranges) {
            auto range_str = std::format("    {}\n", range);
            str.append(range_str);
        }

        return str;
    }

    void register_range(const reservation_type in_type, const physical_addr_t in_start_addr, const std::size_t in_size_bytes);
}; // class map

}; // namespace core::memory

template <>
struct std::formatter<struct core::memory::details::address_range> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const struct core::memory::details::address_range& in_arg) {
        auto typestr = "";
        switch(in_arg.type) {
            case core::memory::reservation_type::UNINITIALIZED:
                typestr = "Uninitialized ";
                break;
            case core::memory::reservation_type::UNALLOCATED:
                typestr = "Unallocated   ";
                break;
            case core::memory::reservation_type::ALLOCATED:
                typestr = "Allocated     ";
                break;
            case core::memory::reservation_type::RESERVED_CORE_DRIVER:
                typestr = "Core Driver   ";
                break;
            case core::memory::reservation_type::RESERVED_DEVICE:
                typestr = "Device        ";
                break;
        }
        auto start = (std::uint64_t)in_arg.start_addr;
        auto end = (start + in_arg.length_bytes) - 1;
        auto range_str = std::format("0x{:016X} - 0x{:016X}: {} [length: {} bytes]", start, end, typestr, in_arg.length_bytes);
        return range_str;
    }
};

template <>
struct std::formatter<core::memory::map> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const core::memory::map& in_arg) {
        return in_arg.format();
    }
};

#endif // _CORE_MEMORY_MAP_HPP