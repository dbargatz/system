#include "logger.hpp"

using namespace kernel::platform::x86_64;

void logger::debug(text& in_msg) {
    _write(vga::color::gray, vga::color::black, "[ ] ", in_msg);
}

void logger::error(text& in_msg) {
    _write(vga::color::white, vga::color::red, "[!] ", in_msg);
}

void logger::info(text& in_msg) {
    _write(vga::color::bright_green, vga::color::black, "[+] ", in_msg);
}

void logger::warn(text& in_msg) {
    _write(vga::color::yellow, vga::color::black, "[-] ", in_msg);
}

void logger::_write(const vga::color in_text_color, 
                    const vga::color in_bg_color, 
                    const char * in_prefix, 
                    text& in_msg) {
    _vga.write(in_prefix, in_text_color, in_bg_color);
    _vga.write(in_msg.get(), in_text_color, in_bg_color);
}
