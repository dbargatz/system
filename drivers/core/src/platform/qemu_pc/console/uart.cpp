#include "uart.hpp"

core::console::uart::uart() {
    // TODO: explain what's going on here
    _INTERRUPT_ENABLE.outb(0x00);
    _LINE_CONTROL.outb(0x80);
    _DATA.outb(0x01);
    _INTERRUPT_ENABLE.outb(0x00);
    _LINE_CONTROL.outb(0x03);
    _ID_FIFO_CONTROL.outb(0xC7);
    _MODEM_CONTROL.outb(0x0B);
}

void core::console::uart::write(const char * in_str) {
    for(int i = 0; in_str[i] != '\0'; i++) {
        // Busy-loop while we wait for space in the FIFO to clear up.
        while(0 == (_LINE_STATUS.inb() & 0x20)) {}

        // Write the current character out.
        _DATA.outb(in_str[i]);
    }
}
