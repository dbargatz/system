#ifndef _CORE_MEMORY_HEAP_HPP
#define _CORE_MEMORY_HEAP_HPP

#include <cassert>
#include <cstdint>
#include <cstring>
#include <new>
#include "../platform.hpp"

namespace core::memory {

typedef std::uint8_t * physical_addr_t;
typedef std::uint8_t * virtual_addr_t;

std::size_t align_to(const std::align_val_t in_boundary, const std::size_t in_num);

struct alignas(16) heap_chunk {
    bool free;
    std::size_t length;
} __attribute__((packed));

constexpr std::align_val_t DEFAULT_ALIGNMENT = (std::align_val_t)16;
constexpr std::size_t MIN_CHUNK_SIZE = (std::size_t)DEFAULT_ALIGNMENT + sizeof(struct heap_chunk);

class heap {
private:
    char _name[16];
    struct heap_chunk * _chunk_list;
    std::size_t _bytes_total;
    std::size_t _bytes_available;
    std::size_t _bytes_used;

public:
    heap(const char in_name[16], physical_addr_t in_initial_frame, std::size_t in_num_frames) {
        assert(((std::uintptr_t)in_initial_frame % PAGE_SIZE_BYTES) == 0);

        std::memcpy((void*)_name, in_name, 16);
        _chunk_list = (struct heap_chunk *)in_initial_frame;
        _chunk_list->length = _bytes_total - sizeof(struct heap_chunk);
        _chunk_list->free = true;

        _bytes_total = PAGE_SIZE_BYTES * in_num_frames;
        _bytes_available = _chunk_list->length;
        _bytes_used = 0;
    }

    physical_addr_t allocate(const std::size_t in_size, const std::align_val_t in_alignment = DEFAULT_ALIGNMENT);
    physical_addr_t reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment = DEFAULT_ALIGNMENT);
    bool deallocate(const physical_addr_t in_addr);
};

}; // namespace core::memory

#endif // _CORE_MEMORY_HEAP_HPP