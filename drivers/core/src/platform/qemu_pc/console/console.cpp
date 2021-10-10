#include "../../../console/console.hpp"
#include "../platform.hpp"
#include "../ports/io_port.hpp"

constexpr static const core::x64::ports::io_port DATA             = core::x64::ports::io_port(COM1_IO_PORT);
constexpr static const core::x64::ports::io_port INTERRUPT_ENABLE = core::x64::ports::io_port(COM1_IO_PORT + 1);
constexpr static const core::x64::ports::io_port ID_FIFO_CONTROL  = core::x64::ports::io_port(COM1_IO_PORT + 2);
constexpr static const core::x64::ports::io_port LINE_CONTROL     = core::x64::ports::io_port(COM1_IO_PORT + 3);
constexpr static const core::x64::ports::io_port MODEM_CONTROL    = core::x64::ports::io_port(COM1_IO_PORT + 4);
constexpr static const core::x64::ports::io_port LINE_STATUS      = core::x64::ports::io_port(COM1_IO_PORT + 5);

core::console::console::console() {
    // TODO: explain what's going on here
    INTERRUPT_ENABLE.outb(0x00);
    LINE_CONTROL.outb(0x80);
    DATA.outb(0x01);
    INTERRUPT_ENABLE.outb(0x00);
    LINE_CONTROL.outb(0x03);
    ID_FIFO_CONTROL.outb(0xC7);
    MODEM_CONTROL.outb(0x0B);
}

void core::console::console::_platform_write(const char * in_str) {
    for(int i = 0; in_str[i] != '\0'; i++) {
        // Busy-loop while we wait for space in the FIFO to clear up.
        while(0 == (LINE_STATUS.inb() & 0x20)) {}

        // Write the current character out.
        DATA.outb(in_str[i]);
    }
}
