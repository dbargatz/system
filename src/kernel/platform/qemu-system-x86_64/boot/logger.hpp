#ifndef KERNEL_PLATFORM_X86_64_LOGGER
#define KERNEL_PLATFORM_X86_64_LOGGER

#include "../types/text.hpp"
#include "vga.hpp"

namespace kernel::platform::x86_64 {
    class logger {
    public:
        logger();

        void debug(types::text& in_msg);
        template<typename ... Args>
        void debug(const char * in_format_str, Args&&... in_args) {
            types::text msg(in_format_str, static_cast<Args>(in_args)...);
            debug(msg);
        }

        void error(types::text& in_msg);
        template<typename ... Args>
        void error(const char * in_format_str, Args&&... in_args) {
            types::text msg(in_format_str, static_cast<Args>(in_args)...);
            error(msg);
        }

        void info(types::text& in_msg);
        template<typename ... Args>
        void info(const char * in_format_str, Args&&... in_args) {
            types::text msg(in_format_str, static_cast<Args>(in_args)...);
            info(msg);
        }

        void panic(types::text& in_msg);
        template<typename ... Args>
        void panic(const char * in_format_str, Args&&... in_args) {
            types::text msg(in_format_str, static_cast<Args>(in_args)...);
            panic(msg);
        }

        void warn(types::text& in_msg);
        template<typename ... Args>
        void warn(const char * in_format_str, Args&&... in_args) {
            types::text msg(in_format_str, static_cast<Args>(in_args)...);
            warn(msg);
        }

    private:
        static vga _vga;

        static void _write(const vga::color in_text_color, 
                           const vga::color in_bg_color, 
                           const char * in_prefix, 
                           types::text& in_msg);
    };
};

#endif