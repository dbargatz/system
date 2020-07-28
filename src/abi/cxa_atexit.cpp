#include "cxxabi.h"
#include <cstdint.hpp>

struct atexit_entry {
    void (*func)(void *);
    void *arg;
    void *dso_handle;
};

constexpr std::uint8_t ATEXIT_MAX_FUNCS = 128;

void * __dso_handle = nullptr;
std::uint8_t __num_atexit_funcs = 0;
struct atexit_entry __atexit_funcs[ATEXIT_MAX_FUNCS] = {{0}};

extern "C" int __cxa_atexit(void (*in_func)(void *), void * in_arg,
                            void * in_dso_handle) {
    if(__num_atexit_funcs >= ATEXIT_MAX_FUNCS) {
        return -1;
    }

    __atexit_funcs[__num_atexit_funcs].func = in_func;
    __atexit_funcs[__num_atexit_funcs].arg = in_arg;
    __atexit_funcs[__num_atexit_funcs].dso_handle = in_dso_handle;
    __num_atexit_funcs++;

    return 0;
}
