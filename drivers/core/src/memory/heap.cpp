#include "heap.hpp"
#include <cassert>

using namespace core::memory;

std::size_t core::memory::align_to(const std::align_val_t in_boundary, const std::size_t in_num) {
    auto remainder = in_num % (std::size_t)in_boundary;
    return (in_num + ((std::size_t)in_boundary - remainder));
}

physical_addr_t heap::allocate(const std::size_t in_size, const std::align_val_t in_alignment) {
    auto aligned_size = align_to(in_alignment, in_size);
    auto cur_addr = (std::uint8_t *)_chunk_list;
    auto max_addr = cur_addr + _bytes_total;

    while(cur_addr < max_addr) {
        auto cur_chunk = (struct heap_chunk *)cur_addr;
        if(cur_chunk->free && cur_chunk->length >= aligned_size) {
            break;
        }
        cur_addr = cur_addr + cur_chunk->length;
    }

    // We iterated the entire heap and couldn't find a chunk big enough for this allocation,
    // either due to actual heap usage or fragmentation.
    if(cur_addr >= max_addr) {
        return nullptr;
    }

    // If the free chunk has enough space left for another heap chunk header plus the
    // alignment size, then create a new free chunk immediately after the current chunk
    // allocation and mark it as free. Otherwise, we'll consume the entire current chunk.
    auto alloc_chunk = (struct heap_chunk *)cur_addr;
    auto remaining_chunk_size = alloc_chunk->length - aligned_size;
    if(remaining_chunk_size >= MIN_CHUNK_SIZE) {
        auto new_free_chunk = (struct heap_chunk *)(cur_addr + aligned_size);
        new_free_chunk->free = true;
        new_free_chunk->length = remaining_chunk_size - sizeof(struct heap_chunk);
        alloc_chunk->length = aligned_size;

        // We created a new heap chunk structure, so subtract that from the available
        // bytes.
        _bytes_available -= sizeof(struct heap_chunk);
    }

    // Mark the chunk as used and clear out the actual heap memory.
    alloc_chunk->free = false;
    auto alloc_mem = cur_addr + sizeof(struct heap_chunk);
    std::memset(alloc_mem, 0, aligned_size);

    // Update the heap statistics.
    _bytes_available -= alloc_chunk->length;
    _bytes_used += aligned_size;
    return alloc_mem;
}

physical_addr_t heap::reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment) {
    return nullptr;
}

bool heap::deallocate(const physical_addr_t in_addr) {
    // Sanity check - ensure the given address is somewhere within the heap boundaries.
    assert(in_addr >= (physical_addr_t)_chunk_list);
    assert(in_addr < ((physical_addr_t)_chunk_list + _bytes_total));

    // Grab the associated heap chunk and ensure it's used (not already free).
    auto chunk = (struct heap_chunk *)(in_addr - sizeof(struct heap_chunk));
    assert(!chunk->free);

    // TODO: Validate that this is a valid heap chunk (heap cookie?)
    // TODO: Coalesce this chunk with any following free chunk.

    // Mark the chunk as free again.
    chunk->free = true;
    return true;
}
