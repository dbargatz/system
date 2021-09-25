#include <new>
#include <cassert>
#include <cstdint>
#include <cstring>

extern void* kernel_heap_bottom;
extern void* kernel_heap_top;
std::uint8_t* cur_heap_ptr = (std::uint8_t *)&kernel_heap_bottom;

void* operator new(std::size_t count) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void* operator new(std::size_t count, std::align_val_t al) {
    auto aligned_size = count + ((std::uint64_t)(cur_heap_ptr + count) % (std::size_t)al);
    std::uint8_t * retval = cur_heap_ptr;

    cur_heap_ptr += aligned_size;
    assert(cur_heap_ptr >= (std::uint8_t *)&kernel_heap_bottom);
    assert(cur_heap_ptr < (std::uint8_t *)&kernel_heap_top);

    std::memset(retval, 0, aligned_size);
    return retval;
}

void* operator new[](std::size_t count) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void* operator new[](std::size_t count, std::align_val_t al) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void operator delete(void* ptr, std::size_t size, std::align_val_t alignment) noexcept {
    assert(ptr >= &kernel_heap_bottom);
    assert(ptr < &kernel_heap_top);
    return;    
}
