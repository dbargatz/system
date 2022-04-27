#include "platform.hpp"

extern "C" std::uint32_t get_el();

std::uint32_t get_permission_level() {
    return get_el();
}