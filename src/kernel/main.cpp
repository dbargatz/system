#include "platform/qemu-system-x86_64/boot/vga.hpp"

extern "C" int kmain(void * inBootInfo) {
    using kernel::platform::x86_64::vga;

    vga output;
    output.clear_screen(vga::color::black);

    char str[2] = { 0x30, 0x00 };
    while (true) {
        // Make sure scrolling works.
        if(str[0] > 0x38) {
            str[0] = 0x30;
        }
        output.write(str, vga::color::white, vga::color::red);
        str[0] += 0x01;
    }
}