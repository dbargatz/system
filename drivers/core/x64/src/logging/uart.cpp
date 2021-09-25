#include "uart.hpp"

bool core::x64::logging::uart::_ourInitialized = false;

core::x64::logging::uart::uart() {
    // TODO: make this test and set atomic
    if(!_ourInitialized) {
        _ourInitialized = true;

        // TODO: explain what's going on here
        _INTERRUPT_ENABLE.outb(0x00);
        _LINE_CONTROL.outb(0x80);
        _DATA.outb(0x01);
        _INTERRUPT_ENABLE.outb(0x00);
        _LINE_CONTROL.outb(0x03);
        _ID_FIFO_CONTROL.outb(0xC7);
        _MODEM_CONTROL.outb(0x0B);
    }
}

void core::x64::logging::uart::write(const std::string& in_str) {
    // TODO: do we need to mutex this somehow? I don't think so, since:
    //         + interrupts disabled until boot/setup complete
    //         + core does nothing other than respond to interrupts/syscalls
    //         + interrupts disabled on entry into handler (due to IDT descriptor type) or syscall handler
    //       This should limit the core driver to doing one thing at a time, so
    //       I don't think we can have concurrency, but I'm sure there are corner cases.
    // TODO: make text implement begin()/end() so this can become a range-based for loop
    for(auto i = 0; i < in_str.length(); i++) {
        // Busy-loop while we wait for space in the FIFO to clear up.
        while(0 == (_LINE_STATUS.inb() & 0x20)) {}

        // Write the current character out.
        _DATA.outb(in_str.c_str()[i]);
    }
}
