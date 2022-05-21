#ifndef _CORE_MEMORY_MANAGER_HPP
#define _CORE_MEMORY_MANAGER_HPP

#include <cassert>
#include <cstdint>
#include <format>
#include <list>
#include "../console/console.hpp"
#include "heap.hpp"


// Level 1: 1 uint64_t = 64 level 2 = 4096k pages, space = 8 bytes
// Level 2: 1 uint64_t = 64 level 3 = 256k pages,  space = 8 * 64 = 512 bytes
// Level 3: 1 uint64_t = 64 level 4 = 4k pages,    space = 8 * 64 * 64 = 32768 bytes
// Level 4: 1 uint64_t = 64 pages,                 space = 8 * 64 * 64 * 64 = 2097152 bytes

namespace core::memory {

enum reservation_type {
    UNINITIALIZED = 0,
    UNALLOCATED = 1,
    ALLOCATED = 2,
    RESERVED_CORE_DRIVER = 3,
    RESERVED_DEVICE = 4,
};

struct page_frame_range {
    std::size_t num_frames : 52;
    std::size_t reserved : 20;
    reservation_type type : 4;
    std::uint64_t start_pfn : 52;
} __attribute__((packed));

class memory_manager {
private:
    heap _core_heap;
    std::list<struct page_frame_range> _page_frames;

public:
    /**
     * @brief Performs initial setup of the memory manager, including setup of the initial core
     * driver heap with the provided arguments. Prior to this constructor returning, no dynamic
     * memory allocation can be performed.
     */
    memory_manager();
    physical_addr_t core_allocate(const std::size_t in_size, const std::align_val_t in_alignment = (const std::align_val_t)16);
    physical_addr_t core_reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment = (const std::align_val_t)16);
    bool core_deallocate(const physical_addr_t in_addr);

    void register_pages(const physical_addr_t in_start, const std::size_t in_size, const reservation_type in_type);

    auto format() const {
        auto str = std::format(
            "Memory Manager at 0x{:X}:\n"
            "  Frame Map    : {} bytes\n"
            "  Ranges       : {}\n",
            (physical_addr_t)this,
            sizeof(_page_frames),
            _page_frames.size()
        );

        for(auto&& range : _page_frames) {
            auto typestr = "";
            switch(range.type) {
                case reservation_type::UNINITIALIZED:
                    typestr = "Uninitialized ";
                    break;
                case reservation_type::UNALLOCATED:
                    typestr = "Unallocated   ";
                    break;
                case reservation_type::ALLOCATED:
                    typestr = "Allocated     ";
                    break;
                case reservation_type::RESERVED_CORE_DRIVER:
                    typestr = "Core Driver   ";
                    break;
                case reservation_type::RESERVED_DEVICE:
                    typestr = "Device        ";
                    break;
            }
            auto start = range.start_pfn << 12;
            auto end = start + (range.num_frames << 12) - 1;
            auto range_str = std::format("    {}: 0x{:X} - 0x{:X}\n", typestr, start, end);
            str.append(range_str);
        }

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