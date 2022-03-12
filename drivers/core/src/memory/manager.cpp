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

/**
 * @brief Performs initial setup of the memory manager, including setup of the initial core
 * driver heap with the provided arguments. Prior to this constructor returning, no dynamic
 * memory allocation can be performed.
 */
memory_manager::memory_manager() : _core_heap(core::memory::heap("core heap", _heap_bytes, _NUM_HEAP_FRAMES)) { }

physical_addr_t memory_manager::core_allocate(const std::size_t in_size, const std::align_val_t in_alignment) {
    return _core_heap.allocate(in_size, in_alignment);
}

physical_addr_t memory_manager::core_reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment) {
    return _core_heap.reserve(in_start, in_size);
}

bool memory_manager::core_deallocate(const physical_addr_t in_addr) {
    return _core_heap.deallocate(in_addr);
}

