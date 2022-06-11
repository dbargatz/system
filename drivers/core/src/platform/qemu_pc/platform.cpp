#include "platform.hpp"
#include "../../core.hpp"

std::uint32_t get_permission_level() {
    return 0;
}

core::core * core::core::get_current_core() {
    return nullptr;
}

void core::core::set_current_core(core * in_core) {
    return;
}