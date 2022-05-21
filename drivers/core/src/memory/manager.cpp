#include "manager.hpp"
#include <cassert>

using namespace core::memory;

// TODO: For now, the initial core heap for the bootstrap core is simply allocated via the BSS
//       section of the core driver binary. Eventually I want all heaps (including this one)
//       allocated from a memory map dynamically, but until I have DeviceTree support for ARM64,
//       this will do.
constexpr std::size_t _NUM_HEAP_FRAMES = 64;
constexpr std::size_t _HEAP_SIZE_BYTES = PAGE_SIZE_BYTES * _NUM_HEAP_FRAMES;
alignas(PAGE_SIZE_BYTES) static std::uint8_t _heap_bytes[_HEAP_SIZE_BYTES] = {0};

memory_manager::memory_manager(std::pmr::memory_resource * in_backing_memory) : _core_heap(core::memory::heap("core heap", _heap_bytes, _NUM_HEAP_FRAMES)) {
    auto listmem = in_backing_memory->allocate(sizeof(*_page_frames));
    auto alloc = frame_range_alloc(in_backing_memory);
    _page_frames = new(listmem) framelist(alloc);
    auto initial_value = page_frame_range {
        .num_frames = (UINT64_MAX >> 12),
        .reserved = 0,
        .type = reservation_type::UNINITIALIZED,
        .start_pfn = 0,
    };
    _page_frames->push_back(initial_value);
}

void memory_manager::register_pages(const physical_addr_t in_start, const std::size_t in_size, const reservation_type in_type) {
    auto alignment = (std::size_t)in_start % PAGE_SIZE_BYTES;
    assertm(alignment == 0, "start address of range is not aligned on page boundary");

    auto num_frames = (in_size >> 12) + (in_size % PAGE_SIZE_BYTES == 0 ? 0 : 1);
    auto value = page_frame_range {
        .num_frames = num_frames,
        .reserved = 0,
        .type = in_type,
        .start_pfn = (std::uint64_t)in_start >> 12,
    };
    _page_frames->push_back(value);
}

physical_addr_t memory_manager::core_allocate(const std::size_t in_size, const std::align_val_t in_alignment) {
    return _core_heap.allocate(in_size, in_alignment);
}

physical_addr_t memory_manager::core_reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment) {
    return _core_heap.reserve(in_start, in_size);
}

bool memory_manager::core_deallocate(const physical_addr_t in_addr) {
    return _core_heap.deallocate(in_addr);
}

