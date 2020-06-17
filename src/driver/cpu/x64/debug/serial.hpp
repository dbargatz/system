#ifndef _DEBUG_SERIAL_HPP
#define _DEBUG_SERIAL_HPP

#include "../ports/io_port.hpp"
#include "../std/text.hpp"

class SerialPort {
private:
    constexpr static const uint16_t _COM1_IO_PORT = 0x03F8;

    constexpr static const io_port _DATA             = io_port(_COM1_IO_PORT);
    constexpr static const io_port _INTERRUPT_ENABLE = io_port(_COM1_IO_PORT + 1);
    constexpr static const io_port _ID_FIFO_CONTROL  = io_port(_COM1_IO_PORT + 2);
    constexpr static const io_port _LINE_CONTROL     = io_port(_COM1_IO_PORT + 3);
    constexpr static const io_port _MODEM_CONTROL    = io_port(_COM1_IO_PORT + 4);
    constexpr static const io_port _LINE_STATUS      = io_port(_COM1_IO_PORT + 5);
    constexpr static const io_port _MODEM_STATUS     = io_port(_COM1_IO_PORT + 6);
    constexpr static const io_port _SCRATCH          = io_port(_COM1_IO_PORT + 7);

    static bool _ourInitialized;

public:
    SerialPort();

    void write(const text& in_text);
};

#endif // _DEBUG_SERIAL_HPP
