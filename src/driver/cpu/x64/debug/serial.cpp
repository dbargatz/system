#include "serial.hpp"

SerialPort::SerialPort(): _data(SerialPort::COM1_IO_PORT),
    _interrupt_enable(SerialPort::COM1_IO_PORT + 1),
    _id_fifo_control(SerialPort::COM1_IO_PORT + 2),
    _line_control(SerialPort::COM1_IO_PORT + 3),
    _modem_control(SerialPort::COM1_IO_PORT + 4),
    _line_status(SerialPort::COM1_IO_PORT + 5),
    _modem_status(SerialPort::COM1_IO_PORT + 6),
    _scratch(SerialPort::COM1_IO_PORT + 7) {
    // TODO: explain what's going on here
    _interrupt_enable.outb(0x00);
    _line_control.outb(0x80);
    _data.outb(0x01);
    _interrupt_enable.outb(0x00);
    _line_control.outb(0x03);
    _id_fifo_control.outb(0xC7);
    _modem_control.outb(0x0B);
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
        while(0 == (_line_status.inb() & 0x20)) {}

        // Write the current character out.
        _data.outb(in_text.get()[i]);
    }
}
