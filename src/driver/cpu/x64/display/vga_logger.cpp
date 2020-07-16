#include "vga_logger.hpp"


vga_logger::vga_logger(vga& in_vga) : _vga(in_vga) {}

void vga_logger::debug(const text& in_msg) {
    _vga.write(in_msg.get(), vga::color::gray, vga::color::black);
}

void vga_logger::error(const text& in_msg) {
    _vga.write(in_msg.get(), vga::color::bright_red, vga::color::black);
}

void vga_logger::info(const text& in_msg) {
    _vga.write(in_msg.get(), vga::color::green, vga::color::black);
}

void vga_logger::panic(const text& in_msg) {
    _vga.write(in_msg.get(), vga::color::white, vga::color::magenta);
}

void vga_logger::warn(const text& in_msg) {
    _vga.write(in_msg.get(), vga::color::yellow, vga::color::black);
}
