#include "heap.hpp"
#include <cassert>

using namespace core::memory;

std::size_t core::memory::align_to(const std::align_val_t in_boundary, const std::size_t in_num) {
    auto remainder = in_num % (std::size_t)in_boundary;
    return (in_num + ((std::size_t)in_boundary - remainder));
}

physical_addr_t heap::allocate(const std::size_t in_size, const std::align_val_t in_alignment) {
    auto aligned_size = align_to(in_alignment, in_size);
    
    if(aligned_size > _bytes_available) {
        _log.warn("cannot allocate {} bytes in heap '{}', only {} bytes remain", aligned_size, (const char *)_name, _bytes_available);
        return nullptr;
    }

    // auto aligned_size = count + ((std::uint64_t)(cur_heap_ptr + count) % (std::size_t)al);
    // std::uint8_t * retval = cur_heap_ptr;

    // cur_heap_ptr += aligned_size;
    // assert(cur_heap_ptr >= _early_heap_bottom);
    // assert(cur_heap_ptr < _early_heap_top);

    // std::memset(retval, 0, aligned_size);

    return nullptr;
}

physical_addr_t heap::reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment) {
    return nullptr;
}

bool heap::deallocate(const physical_addr_t in_addr) {
    return false;
}
