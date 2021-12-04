#ifndef _CORE_MEMORY_HEAP_HPP
#define _CORE_MEMORY_HEAP_HPP

#include <cassert>
#include <cstdint>
#include <cstring>
#include "../console/console.hpp"
#include "../platform.hpp"

namespace core::memory {

typedef std::uint8_t * physical_addr_t;
typedef std::uint8_t * virtual_addr_t;

struct alignas(16) heap_chunk {
    std::uint8_t * next;
    std::uint8_t * prev;
} __attribute__((packed));

class heap {
private:
    char _name[16];
    const core::console::console& _log;
    const struct heap_chunk * _start;
    const struct heap_chunk * _next_free;
    std::size_t _bytes_total;
    std::size_t _bytes_available;
    std::size_t _bytes_used;

public:
    heap(const char in_name[16], const core::console::console& in_log, physical_addr_t in_initial_frame) : _log(in_log) {
        assert(((std::uintptr_t)in_initial_frame % PAGE_SIZE_BYTES) == 0);

        _start = (struct heap_chunk *)in_initial_frame;
        std::memcpy((void*)_name, in_name, 16);

        _bytes_total = PAGE_SIZE_BYTES;
        _bytes_available = PAGE_SIZE_BYTES;
        _bytes_used = 0;
    }

    physical_addr_t allocate(const std::size_t in_size);
    physical_addr_t allocate(const physical_addr_t in_start, const std::size_t in_size);
    bool deallocate(const physical_addr_t in_addr);
};

}; // namespace core::memory

#endif // _CORE_MEMORY_HEAP_HPP