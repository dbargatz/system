#ifndef _CORE_MEMORY_HEAP_HPP
#define _CORE_MEMORY_HEAP_HPP

#include <cassert>
#include <cstdint>
#include <cstring>
#include <format>
#include <new>
#include "types.hpp"
#include "../platform.hpp"

namespace core::memory {

std::size_t align_to(const std::align_val_t in_boundary, const std::size_t in_num);

struct heap_chunk {
    bool free          : 1;
    std::size_t length : 63;
} __attribute__((packed));

constexpr std::align_val_t DEFAULT_ALIGNMENT = (std::align_val_t)sizeof(struct heap_chunk);
constexpr std::size_t MIN_CHUNK_SIZE = (std::size_t)DEFAULT_ALIGNMENT + sizeof(struct heap_chunk);

class heap {
private:
    // Name of heap for debugging purposes.
    char _name[16];

    // Physical start address of the contiguous memory range this heap can allocate/reserve from.
    physical_addr_t _heap_start;

    // Physical end address of the contiguous memory range this heap can allocate/reserve from.
    physical_addr_t _heap_end;

    // Number of heap bytes currently allocated/reserved to heap callers. Includes alignment bytes,
    // as those bytes are technically usable by a caller without trashing heap_chunk structures.
    std::size_t _bytes_used;

    // Number of heap bytes currently used for chunk headers and internal fragmentation, meaning
    // they aren't usable by heap callers.
    std::size_t _bytes_overhead;

public:
    heap(const char in_name[16], physical_addr_t in_initial_frame, std::size_t in_num_frames) {
        // Sanity check - ensure the contiguous memory region given starts on a page boundary.
        assert(((std::size_t)in_initial_frame % PAGE_SIZE_BYTES) == 0);

        // Initialize the debug name and calculate the heap extents.
        std::memcpy((void*)_name, in_name, 16);
        auto length = (PAGE_SIZE_BYTES * in_num_frames);
        _heap_start = in_initial_frame;
        _heap_end = _heap_start + length;

        // Allocate the first chunk at the start of the heap, marking it as free but subtracting
        // the chunk header size from the length.
        auto first_chunk = (struct heap_chunk *)_heap_start;
        first_chunk->free = true;
        first_chunk->length = length - sizeof(*first_chunk);

        // Initialize the statistics vars - none of the bytes are used yet because nothing has
        // asked the heap to allocate/reserve bytes yet, but we do have overhead bytes already,
        // since we created a chunk header!
        _bytes_used = 0;
        _bytes_overhead = sizeof(*first_chunk);
    }

    physical_addr_t allocate(const std::size_t in_size, const std::align_val_t in_alignment = DEFAULT_ALIGNMENT);
    physical_addr_t reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment = DEFAULT_ALIGNMENT);
    bool deallocate(const physical_addr_t in_addr);

    auto format() const {
        auto total = (std::size_t)(_heap_end - _heap_start);
        auto available = total - _bytes_used - _bytes_overhead;
        return std::format(
            "Heap '{}' at 0x{:016X}:\n"
            "  Buffer   : 0x{:016X}\n"
            "  Total    : {} bytes\n"
            "  Available: {} bytes\n"
            "  Used     : {} bytes\n"
            "  Overhead : {} bytes",
            (const char *)_name, (physical_addr_t)this, (physical_addr_t)_heap_start,
            total, available, _bytes_used, _bytes_overhead
        );
    }
};

}; // namespace core::memory

template <>
struct std::formatter<core::memory::heap> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const core::memory::heap& in_arg) {
        return in_arg.format();
    }
};

#endif // _CORE_MEMORY_HEAP_HPP