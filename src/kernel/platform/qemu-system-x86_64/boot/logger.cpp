#include "logger.hpp"

using namespace kernel::platform::x86_64;

vga logger::_vga;

logger::logger() {
    _vga.clear_screen(vga::color::black);
}

void logger::debug(types::text& in_msg) {
    _write(vga::color::gray, vga::color::black, "[ ] ", in_msg);
}

void logger::error(types::text& in_msg) {
    _write(vga::color::white, vga::color::red, "[!] ", in_msg);
}

void logger::info(types::text& in_msg) {
    _write(vga::color::bright_green, vga::color::black, "[+] ", in_msg);
}

void logger::panic(types::text& in_msg) {
    _vga.clear_screen(vga::color::bright_magenta);
    _vga.set_position(0, 0);
    _write(vga::color::white, vga::color::bright_magenta, "PANIC: ", in_msg);
}

void logger::warn(types::text& in_msg) {
    _write(vga::color::yellow, vga::color::black, "[-] ", in_msg);
}

void logger::_write(const vga::color in_text_color, 
                    const vga::color in_bg_color, 
                    const char * in_prefix, 
                    types::text& in_msg) {
    _vga.write(in_prefix, in_text_color, in_bg_color);
    _vga.write(in_msg.get(), in_text_color, in_bg_color);
}