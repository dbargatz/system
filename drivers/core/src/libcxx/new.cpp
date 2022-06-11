#include <new>
#include <cassert>
#include <cstdint>
#include <cstring>
#include "../core.hpp"

void* operator new(std::size_t count) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void* operator new(std::size_t count, void* ptr) noexcept {
    return ptr;
}

void* operator new[](std::size_t count, void* ptr) noexcept {
    return ptr;
}

void* operator new(std::size_t count, std::align_val_t al) {
    auto core = core::core::get_current_core();
    return core->memory_manager.core_allocate(count, al);
}

void* operator new[](std::size_t count) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void* operator new[](std::size_t count, std::align_val_t al) {
    return ::operator new(count, std::align_val_t(alignof(std::max_align_t)));
}

void operator delete(void* ptr) noexcept {
    auto core = core::core::get_current_core();
    core->memory_manager.core_deallocate((core::memory::physical_addr_t)ptr);
    return;
}

void operator delete(void* ptr, std::size_t size, std::align_val_t alignment) noexcept {
    auto core = core::core::get_current_core();
    core->memory_manager.core_deallocate((core::memory::physical_addr_t)ptr);
    return;
}
