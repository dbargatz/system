#include "serial.hpp"
#include "../ports/asm.h"

SerialPort::SerialPort(logger& in_log) : _log(in_log) {
    outb(INTERRUPT_ENABLE, 0x00);
    outb(LINE_CONTROL    , 0x80);
    outb(DATA            , 0x01);
    outb(INTERRUPT_ENABLE, 0x00);
    outb(LINE_CONTROL    , 0x03);
    outb(ID_FIFO_CONTROL , 0xC7);
    outb(MODEM_CONTROL   , 0x0B);
    _log.debug("Serial port initialized.\n");
}

void SerialPort::write(const text& in_text) {
    // TODO: make text implement begin()/end() so this can become a range-based for loop
    for(size_t i = 0; i < in_text.length(); i++) {
        // Busy-loop while we wait for space in the FIFO to clear up.
        while(0 == (inb(LINE_STATUS) & 0x20)) {}

        // Write the current character out.
        outb(DATA, in_text.get()[i]);
    }
}
