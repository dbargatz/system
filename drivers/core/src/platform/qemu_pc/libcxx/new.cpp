#include <new>
#include <cassert>
#include <cstdint>
#include <cstring>

std::uint8_t _early_heap[65535] = {0};

std::uint8_t* _early_heap_bottom = (std::uint8_t *)&_early_heap;
std::uint8_t* _early_heap_top = _early_heap_bottom + 65535;
std::uint8_t* cur_heap_ptr = _early_heap_bottom;

void* operator new(std::size_t count) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void* operator new(std::size_t count, std::align_val_t al) {
    auto aligned_size = count + ((std::uint64_t)(cur_heap_ptr + count) % (std::size_t)al);
    std::uint8_t * retval = cur_heap_ptr;

    cur_heap_ptr += aligned_size;
    assert(cur_heap_ptr >= (std::uint8_t *)&_early_heap_bottom);
    assert(cur_heap_ptr < (std::uint8_t *)&_early_heap_top);

    std::memset(retval, 0, aligned_size);
    return retval;
}

void* operator new[](std::size_t count) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void* operator new[](std::size_t count, std::align_val_t al) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void operator delete(void* ptr) noexcept {
    assert(ptr >= &_early_heap_bottom);
    assert(ptr < &_early_heap_top);
    return;    
}

void operator delete(void* ptr, std::size_t size, std::align_val_t alignment) noexcept {
    assert(ptr >= &_early_heap_bottom);
    assert(ptr < &_early_heap_top);
    return;    
}
