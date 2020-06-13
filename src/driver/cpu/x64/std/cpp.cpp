#include "cpp.hpp"
#include "panic.h"

extern "C" void __cxa_pure_virtual() {
    PANIC("Pure virtual function called!\n");
}
