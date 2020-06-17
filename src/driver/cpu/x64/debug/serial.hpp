#ifndef _DEBUG_SERIAL_HPP
#define _DEBUG_SERIAL_HPP

#include "../ports/io_port.hpp"
#include "../std/text.hpp"

class SerialPort {
private:
    static const uint16_t COM1_IO_PORT = 0x03F8;

    io_port _data;
    io_port _interrupt_enable;
    io_port _id_fifo_control;
    io_port _line_control;
    io_port _modem_control;
    io_port _line_status;
    io_port _modem_status;
    io_port _scratch;

public:
    SerialPort();

    void write(const text& in_text);
};

#endif // _DEBUG_SERIAL_HPP
