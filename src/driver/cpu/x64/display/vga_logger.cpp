#include "vga_logger.hpp"

void vga_backend::write(const std::string& in_str) {
    _vga.write(in_str, vga::color::gray, vga::color::black);
}
