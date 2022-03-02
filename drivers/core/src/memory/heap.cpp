#include "heap.hpp"
#include <cassert>

using namespace core::memory;

physical_addr_t heap::allocate(const std::size_t in_size) {
    auto aligned_size = align_to(16, in_size);
    
    if(aligned_size > _bytes_available) {
        _log.warn("cannot allocate {} bytes in heap '{}', only {} bytes remain", aligned_size, (const char *)_name, _bytes_available);
        return nullptr;
    }

    return nullptr;
}

physical_addr_t heap::allocate(const physical_addr_t in_start, const std::size_t in_size) {
    return nullptr;
}

bool heap::deallocate(const physical_addr_t in_addr) {
    return false;
}
