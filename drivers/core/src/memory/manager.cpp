#include "manager.hpp"
#include <cassert>
#include "../platform.hpp"

using namespace core::memory;

alignas(PAGE_SIZE_BYTES) static std::uint8_t _assert_heap_buffer[PAGE_SIZE_BYTES] = {0};
alignas(PAGE_SIZE_BYTES) static std::uint8_t _core_heap_buffer[PAGE_SIZE_BYTES * 8] = {0};

// This is defined in libcxx/cassert.cpp and is used to determine if the assert heap should be used.
extern bool _assert_in_progress;

memory_manager::memory_manager() : 
    _assert_heap(_assert_heap_buffer, sizeof(_assert_heap_buffer)),
    _core_heap(_core_heap_buffer, sizeof(_core_heap_buffer)),
    _memory_map(&_core_heap) {
    _memory_map.register_range(reservation_type::UNINITIALIZED, 0, UINT64_MAX);
}

void memory_manager::register_range(const physical_addr_t in_start, const std::size_t in_size, const reservation_type in_type) {
    auto alignment = (std::size_t)in_start % PAGE_SIZE_BYTES;
    assertm(alignment == 0, "start address of range is not aligned on page boundary");
    _memory_map.register_range(in_type, in_start, in_size);
}

physical_addr_t memory_manager::core_allocate(const std::size_t in_size, const std::align_val_t in_alignment) {
    if(_assert_in_progress) {
        return (physical_addr_t)_assert_heap.allocate(in_size, in_alignment);
    }
    return (physical_addr_t)_core_heap.allocate(in_size, in_alignment);
}

void memory_manager::core_deallocate(const physical_addr_t in_addr) {
    if(_assert_in_progress) {
        return _assert_heap.deallocate((void *)in_addr, 0);
    }
    _core_heap.deallocate((void *)in_addr, 0);
}
