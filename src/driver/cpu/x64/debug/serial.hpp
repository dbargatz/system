#ifndef _DEBUG_SERIAL_HPP
#define _DEBUG_SERIAL_HPP

#include "../ports/io.h"
#include "../std/logger.hpp"
#include "../std/text.hpp"

class SerialPort {
private:
    static const io_port_addr_t COM1_IO_PORT      = 0x03F8;
    static const io_port_addr_t DATA              = COM1_IO_PORT;
    static const io_port_addr_t INTERRUPT_ENABLE  = DATA + 1;
    static const io_port_addr_t ID_FIFO_CONTROL   = DATA + 2;
    static const io_port_addr_t LINE_CONTROL      = DATA + 3;
    static const io_port_addr_t MODEM_CONTROL     = DATA + 4;
    static const io_port_addr_t LINE_STATUS       = DATA + 5;
    static const io_port_addr_t MODEM_STATUS      = DATA + 6;
    static const io_port_addr_t SCRATCH           = DATA + 7;

    logger& _log;

public:
    SerialPort(logger& in_log);

    void write(const text& in_text);
};

#endif // _DEBUG_SERIAL_HPP
