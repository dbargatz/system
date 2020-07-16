#ifndef _DISPLAY_VGA_LOGGER_HPP
#define _DISPLAY_VGA_LOGGER_HPP

#include "vga.hpp"
#include "../std/logger_backend.hpp"

class vga_logger : public logger_backend {
public:
    vga_logger(vga& in_vga);

    void debug(const text& in_msg) override;
    void error(const text& in_msg) override;
    void info(const text& in_msg) override;
    void panic(const text& in_msg) override;
    void warn(const text& in_msg) override;

private:
    vga& _vga;
};

#endif // _DISPLAY_VGA_LOGGER_HPP
