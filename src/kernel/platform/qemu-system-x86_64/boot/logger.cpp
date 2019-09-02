#include "logger.hpp"

using namespace kernel::platform::x86_64;

vga logger::_vga;

#define VA_WRAPPER(text, bg, prefix, msg) __builtin_va_list args; \
    __builtin_va_start(args, in_msg); \
    write(text, bg, prefix, msg, args); \
    __builtin_va_end(args)

logger::logger() {
    _vga.clear_screen(vga::color::black);
}

void logger::dbg(const char * in_msg, ...) {
    VA_WRAPPER(vga::color::gray, vga::color::black, "[ ] ", in_msg);
}

void logger::err(const char * in_msg, ...) {
    VA_WRAPPER(vga::color::white, vga::color::red, "[!] ", in_msg);
}

void logger::inf(const char * in_msg, ...) {
    VA_WRAPPER(vga::color::bright_green, vga::color::black, "[+] ", in_msg);
}

void logger::wrn(const char * in_msg, ...) {
    VA_WRAPPER(vga::color::yellow, vga::color::black, "[-] ", in_msg);
}

void logger::write(const vga::color in_text_color, 
        const vga::color in_bg_color, const char * in_prefix, 
        const char * in_msg, __builtin_va_list in_varargs) {
    _vga.write(in_prefix, in_text_color, in_bg_color);
    _vga.write(in_msg, in_text_color, in_bg_color);
}