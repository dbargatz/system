#include <memory_resource>

std::pmr::new_delete_memory_resource _new_delete_resource;
std::pmr::memory_resource* _default_memory_resource = &_new_delete_resource;

std::pmr::memory_resource* std::pmr::get_default_resource() noexcept {
    return _default_memory_resource;
}

std::pmr::memory_resource* std::pmr::new_delete_resource() noexcept {
    return &_new_delete_resource;
}

std::pmr::memory_resource* std::pmr::set_default_resource(std::pmr::memory_resource* r) noexcept {
    auto prev = _default_memory_resource;
    if(nullptr == r) {
        _default_memory_resource = new_delete_resource();
    } else {
        _default_memory_resource = r;
    }
    return prev;
}
