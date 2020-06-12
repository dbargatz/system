#include "serial.hpp"
#include "../ports/asm.h"

bool SerialPort::_our_initialized = false;

SerialPort::SerialPort() {
    if(!_our_initialized) {
        // TODO: make this check/set atomic
        _our_initialized = true;

        // TODO: add comments
        outb(INTERRUPT_ENABLE, 0x00);
        outb(LINE_CONTROL    , 0x80);
        outb(DATA            , 0x01);
        outb(INTERRUPT_ENABLE, 0x00);
        outb(LINE_CONTROL    , 0x03);
        outb(ID_FIFO_CONTROL , 0xC7);
        outb(MODEM_CONTROL   , 0x0B);
    }
}

void SerialPort::write(const text& in_text) {
    // TODO: do we need to mutex this somehow? I don't think so, since:
    //         + interrupts disabled until boot/setup complete
    //         + core does nothing other than respond to interrupts/syscalls
    //         + interrupts disabled on entry into handler (due to IDT descriptor type) or syscall handler
    //       This should limit the core driver to doing one thing at a time, so
    //       I don't think we can have concurrency, but I'm sure there are corner cases.
    // TODO: make text implement begin()/end() so this can become a range-based for loop
    for(size_t i = 0; i < in_text.length(); i++) {
        // Busy-loop while we wait for space in the FIFO to clear up.
        while(0 == (inb(LINE_STATUS) & 0x20)) {}

        // Write the current character out.
        outb(DATA, in_text.get()[i]);
    }
}
