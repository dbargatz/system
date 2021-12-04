#ifndef _CORE_MEMORY_MANAGER_HPP
#define _CORE_MEMORY_MANAGER_HPP

#include <cassert>
#include <cstdint>
#include "../console/console.hpp"
#include "heap.hpp"

namespace core::memory {

std::size_t align_to(const std::size_t in_boundary, const std::size_t in_num) {
    auto remainder = in_num % in_boundary;
    return (in_num + (in_num - remainder));
}

class memory_manager {
private:
    const core::console::console& _log;
    const heap * _core_heap;

public:
    /**
     * @brief Performs initial setup of the memory manager, including setup of the initial core
     * driver heap with the provided arguments. Prior to this constructor returning, no dynamic
     * memory allocation can be performed.
     * 
     * @param in_heap_start pointer to the start of a 64KB+ readable/writeable memory area to serve
     * as the early core driver heap; must be aligned on 16-byte boundary
     * @param in_heap_size size of the in_heap memory area in bytes; must be at least 64KB and a
     * multiple of 16
     */
    memory_manager(core::console::console& in_log, const std::uint8_t * in_page_frames, std::size_t in_num_frames) : _log(in_log) {
        assert(((std::uintptr_t)in_page_frames % PAGE_SIZE_BYTES) == 0);
        assert(in_num_frames >= 16);

        auto space_for_heap_obj = align_to(16, sizeof(heap));
        auto first_chunk_start = (physical_addr_t)in_heap + space_for_heap_obj;
        auto new_heap_len = in_heap_len - space_for_heap_obj;
        _core_heap = new((void *)in_heap) heap("early core", _log, first_chunk_start, new_heap_len);
    }

    

};

}; // namespace core::memory

#endif // _CORE_MEMORY_MANAGER_HPP