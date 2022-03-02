#include "manager.hpp"
#include <cassert>

using namespace core::memory;

physical_addr_t memory_manager::core_allocate(const std::size_t in_size, const std::align_val_t in_alignment) {
    return _core_heap.allocate(in_size, in_alignment);
}

physical_addr_t memory_manager::core_reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment) {
    return _core_heap.reserve(in_start, in_size);
}

bool memory_manager::core_deallocate(const physical_addr_t in_addr) {
    return _core_heap.deallocate(in_addr);
}
