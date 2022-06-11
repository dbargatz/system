#include <cassert>
#include <cstdlib>
#include "../core.hpp"

void __assert_failed(const char * in_file, int in_line, const char * in_func, const char * in_msg) {
    auto current_core = core::core::get_current_core();
    current_core->handle_assert(in_file, in_line, in_func, in_msg);
}
