#ifndef _CORE_MEMORY_MANAGER_HPP
#define _CORE_MEMORY_MANAGER_HPP

#include <cassert>
#include <cstdint>
#include "../console/console.hpp"
#include "heap.hpp"

namespace core::memory {

class memory_manager {
private:
    core::console::console& _log;
    heap _core_heap;

public:
    /**
     * @brief Performs initial setup of the memory manager, including setup of the initial core
     * driver heap with the provided arguments. Prior to this constructor returning, no dynamic
     * memory allocation can be performed.
     */
    memory_manager(core::console::console& in_log, physical_addr_t in_core_heap_bytes, std::size_t in_num_frames) : _log(in_log), _core_heap(core::memory::heap("core heap", in_log, in_core_heap_bytes, in_num_frames)) { }

    physical_addr_t core_allocate(const std::size_t in_size, const std::align_val_t in_alignment = (const std::align_val_t)16);
    physical_addr_t core_reserve(const physical_addr_t in_start, const std::size_t in_size, const std::align_val_t in_alignment = (const std::align_val_t)16);
    bool core_deallocate(const physical_addr_t in_addr);
};

}; // namespace core::memory

#endif // _CORE_MEMORY_MANAGER_HPP