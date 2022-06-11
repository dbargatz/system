#ifndef _CORE_CORE_HPP
#define _CORE_CORE_HPP

#include <cassert>
#include <cstdlib>
#include "console/console.hpp"
#include "memory/manager.hpp"

namespace core {

class core {
public:
    console::console& log;
    memory::memory_manager& memory_manager;

    static core * get_current_core();
    static void set_current_core(core * in_core);

    core() = delete;
    core(console::console& in_log, memory::memory_manager& in_mem_mgr) : log(in_log), memory_manager(in_mem_mgr) {
        log.info("Initialized core.");
    }

    [[noreturn]] void handle_assert(const char * in_file, int in_line, const char * in_func, const char * in_msg) {
        memory_manager.use_assert_heap();
        log.error("{} ({}:{}:{}())", in_msg, in_file, in_line, in_func);
        std::abort();
    }
};

}; // namespace core

#endif // _CORE_CORE_HPP