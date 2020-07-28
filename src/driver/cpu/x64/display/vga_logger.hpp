#ifndef _DISPLAY_VGA_LOGGER_HPP
#define _DISPLAY_VGA_LOGGER_HPP

#include <string>
#include "../../../../logging/backend.hpp"
#include "vga.hpp"

class vga_backend : public logging::backend {
public:
    vga_backend(vga& in_vga) : _vga(in_vga) {}

    void write(const std::string& in_str) override;

private:
    vga& _vga;
};

#endif // _DISPLAY_VGA_LOGGER_HPP
