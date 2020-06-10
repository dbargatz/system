#ifndef _STD_LOGGER_HPP
#define _STD_LOGGER_HPP

#include "text.hpp"
#include "../display/vga.hpp"

class logger {
public:
    logger(vga& in_vga) : _vga(in_vga) {}

    void debug(text& in_msg);
    template<typename ... Args>
    void debug(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        debug(msg);
    }

    void error(text& in_msg);
    template<typename ... Args>
    void error(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        error(msg);
    }

    void info(text& in_msg);
    template<typename ... Args>
    void info(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        info(msg);
    }

    void warn(text& in_msg);
    template<typename ... Args>
    void warn(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        warn(msg);
    }

private:
    vga& _vga;

    void _write(const vga::color in_text_color,
                const vga::color in_bg_color,
                const char * in_prefix,
                text& in_msg);
};

#endif // _STD_LOGGER_HPP
