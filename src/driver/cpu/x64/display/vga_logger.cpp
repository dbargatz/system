#include "vga_logger.hpp"

vga vga_logger::_our_vga;

void vga_logger::debug(const text& in_msg) {
    _our_vga.write(in_msg.get(), vga::color::gray, vga::color::black);
}

void vga_logger::error(const text& in_msg) {
    _our_vga.write(in_msg.get(), vga::color::bright_red, vga::color::black);
}

void vga_logger::info(const text& in_msg) {
    _our_vga.write(in_msg.get(), vga::color::green, vga::color::black);
}

void vga_logger::panic(const text& in_msg) {
    _our_vga.write(in_msg.get(), vga::color::white, vga::color::magenta);
}

void vga_logger::warn(const text& in_msg) {
    _our_vga.write(in_msg.get(), vga::color::yellow, vga::color::black);
}
