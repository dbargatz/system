#include "heap.hpp"
#include <cassert>

using namespace core::memory;

physical_addr_t heap::allocate(const std::size_t in_size) {
    if(in_size % 16 != 0) {
        _log.error("in_size was not a multiple of 16");
        return nullptr;
    } else if(in_size > _bytes_available) {
        _log.error("cannot allocate {} bytes in heap '{}', only {} bytes remain", in_size, _name, _bytes_available);
        return nullptr;
    }

    
}

physical_addr_t heap::allocate(const physical_addr_t in_start, const std::size_t in_size) {

}

bool heap::deallocate(const physical_addr_t in_addr) {

}
