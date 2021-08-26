#include <new>
#include <cassert>
#include <cstdint>

extern void* kernel_heap_bottom;
extern void* kernel_heap_top;
std::uint8_t* cur_heap_ptr = (std::uint8_t *)kernel_heap_bottom;

void* operator new(std::size_t size) {
    return ::operator new(size, sizeof(std::max_align_t));
}

void* operator new(std::size_t size, std::align_val_t alignment) {
    std::uint8_t * retval = cur_heap_ptr;
    std::uint8_t * new_ptr = cur_heap_ptr + size;
    new_ptr += ((std::uint64_t)new_ptr % alignment);
    assert(new_ptr < kernel_heap_top);
    return retval;
}

void operator delete(void* ptr, std::size_t size, std::align_val_t alignment) noexcept {
    assert(ptr >= kernel_heap_bottom);
    assert(ptr < kernel_heap_top);
    return;    
}
