#include "heap.hpp"
#include <cassert>

using namespace core::memory;

std::size_t core::memory::align_to(const std::align_val_t in_alignment, const std::size_t in_num) {
    auto align = (std::size_t)in_alignment;
    auto remainder = in_num % align;
    return (in_num + ((align - remainder) % align));
}

physical_addr_t heap::allocate(const std::size_t in_size, const std::align_val_t in_alignment) {
    // Sanity check - don't allow allocations of zero size.
    assert(in_size != 0);

    // Align the allocation up to meet alignment requirements.
    auto aligned_size = align_to(in_alignment, in_size);
    auto cur_addr = _heap_start;
    auto cur_chunk = (struct heap_chunk *)_heap_start;

    // Walk through the heap chunks until we find a chunk large enough to fit the required
    // aligned allocation, or until we reach the end of the heap. Note that this is a terrible,
    // naiive "first fit" algorithm - it manages to be O(n) AND horribly space-inefficient!
    while(cur_addr < _heap_end) {
        if(cur_chunk->free && cur_chunk->length >= aligned_size) {
            break;
        }
        cur_addr += sizeof(*cur_chunk) + cur_chunk->length;
        cur_chunk = (struct heap_chunk *)cur_addr;
    }

    // We iterated the entire heap and couldn't find a chunk big enough for this allocation,
    // either due to actual heap usage or fragmentation.
    assert(cur_addr < _heap_end);

    // If the free chunk has enough space left for another heap chunk header plus the
    // alignment size, then create a new free chunk immediately after the current chunk
    // allocation and mark it as free. Otherwise, we'll consume the entire current chunk.
    auto remaining_chunk_size = cur_chunk->length - aligned_size;
    if(remaining_chunk_size >= MIN_CHUNK_SIZE) {
        // Create a new heap chunk for the memory from the current chunk we don't need.
        auto new_free_chunk = (struct heap_chunk *)(cur_addr + sizeof(struct heap_chunk) + aligned_size);
        new_free_chunk->free = true;
        new_free_chunk->wasted = 0;
        new_free_chunk->length = remaining_chunk_size - sizeof(struct heap_chunk);

        // Update the overhead statistic - we created a new heap chunk, which isn't usable space.
        _bytes_overhead += sizeof(struct heap_chunk);
        cur_chunk->length = aligned_size;
        _num_chunks++;
    } else {
        // We don't create a new chunk, but the space remaining in the current chunk is technically
        // overhead (aka internal fragmentation), because it's not usable space until the current
        // chunk is freed. Count it as overhead!
        cur_chunk->wasted = remaining_chunk_size;
        _bytes_overhead += remaining_chunk_size;
    }

    // Mark the current chunk as used and clear out the actual heap memory.
    cur_chunk->free = false;
    auto alloc_mem = cur_addr + sizeof(struct heap_chunk);
    std::memset(alloc_mem, 0, aligned_size);

    // Update the amount of memory actually used by the allocation; we count alignment bytes as
    // part of the allocation, unlike chunk headers or internal fragmentation.
    auto actually_used = cur_chunk->length - cur_chunk->wasted;
    _bytes_used += actually_used;
    return alloc_mem;
}

physical_addr_t heap::reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment) {
    return nullptr;
}

bool heap::deallocate(const physical_addr_t in_addr) {
    // Sanity check - we should never be deallocating nullptr.
    assert(in_addr != nullptr);

    // Sanity check - ensure the given address is somewhere within the heap boundaries, and is
    // marked as free.
    auto chunk_addr = in_addr - sizeof(struct heap_chunk);
    auto chunk = (struct heap_chunk *)chunk_addr;
    assert(chunk_addr >= _heap_start);
    assert(in_addr < _heap_end);
    assert(!chunk->free);

    // Update the bytes used before we start coalescing chunks.
    auto actually_used = chunk->length - chunk->wasted;
    _bytes_used -= actually_used;
    _bytes_overhead -= chunk->wasted;

    // Coalesce this chunk with all contiguous following free chunks.
    auto next_chunk_addr = chunk_addr + sizeof(*chunk) + chunk->length;
    auto next_chunk = (struct heap_chunk *)next_chunk_addr;
    while(next_chunk_addr < _heap_end && next_chunk->free) {
        auto additional_length = sizeof(*next_chunk) + next_chunk->length;
        chunk->length += additional_length;
        _bytes_overhead -= sizeof(*next_chunk);
        _num_chunks -= 1;
        next_chunk_addr += additional_length;
        next_chunk = (struct heap_chunk *)next_chunk_addr;
    }

    // Mark the chunk as free again with no wasted space.
    chunk->free = true;
    chunk->wasted = 0;
    return true;
}
