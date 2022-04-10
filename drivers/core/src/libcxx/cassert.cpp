#include <cassert>
#include <cstdlib>
#include "../console/console.hpp"

core::console::console * _core_assert_log;

void __assert_failed(const char * in_file, int in_line, const char * in_func, const char * in_msg) {
    _core_assert_log->error("{} ({}:{}:{}())", in_msg, in_file, in_line, in_func);
    std::abort();
}
