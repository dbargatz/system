#include "platform.hpp"
#include "../../core.hpp"

extern "C" std::uint32_t get_el();
extern "C" std::uint64_t get_tpidr_el2();
extern "C" void set_tpidr_el2(std::uint64_t in_addr);

std::uint32_t get_permission_level() {
    return get_el();
}

core::core * core::core::get_current_core() {
    return (core *)get_tpidr_el2();
}

void core::core::set_current_core(core * in_core) {
    set_tpidr_el2((std::uint64_t)in_core);
}