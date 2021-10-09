#ifndef _CORE_PLATFORM_QEMU_PC_CONSOLE_UART_HPP
#define _CORE_PLATFORM_QEMU_PC_CONSOLE_UART_HPP

#include <cstddef>
#include <cstdint>
#include "../ports/io_port.hpp"

namespace core::console {

class uart {
private:
    constexpr static const std::uint16_t _COM1_IO_PORT = 0x03F8;

    constexpr static const x64::ports::io_port _DATA             = x64::ports::io_port(_COM1_IO_PORT);
    constexpr static const x64::ports::io_port _INTERRUPT_ENABLE = x64::ports::io_port(_COM1_IO_PORT + 1);
    constexpr static const x64::ports::io_port _ID_FIFO_CONTROL  = x64::ports::io_port(_COM1_IO_PORT + 2);
    constexpr static const x64::ports::io_port _LINE_CONTROL     = x64::ports::io_port(_COM1_IO_PORT + 3);
    constexpr static const x64::ports::io_port _MODEM_CONTROL    = x64::ports::io_port(_COM1_IO_PORT + 4);
    constexpr static const x64::ports::io_port _LINE_STATUS      = x64::ports::io_port(_COM1_IO_PORT + 5);
    constexpr static const x64::ports::io_port _MODEM_STATUS     = x64::ports::io_port(_COM1_IO_PORT + 6);
    constexpr static const x64::ports::io_port _SCRATCH          = x64::ports::io_port(_COM1_IO_PORT + 7);

    static bool _ourInitialized;

public:
    uart();

    void write(const char* in_str);
};

};

#endif // _CORE_PLATFORM_QEMU_PC_CONSOLE_UART_HPP
