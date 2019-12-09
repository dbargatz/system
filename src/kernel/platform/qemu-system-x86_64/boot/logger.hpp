#ifndef KERNEL_PLATFORM_X86_64_LOGGER
#define KERNEL_PLATFORM_X86_64_LOGGER

#include "vga.hpp"

namespace kernel::platform::x86_64 {
    class logger {
    public:
        logger();

        void debug(const char * in_msg, ...);
        void error(const char * in_msg, ...);
        void info(const char * in_msg, ...);
        void warn(const char * in_msg, ...);

    private:
        static vga _vga;

        static void _write(const vga::color in_text_color, 
            const vga::color in_bg_color, const char * in_prefix, 
            const char * in_msg, __builtin_va_list in_varargs);
    };
};

#endif