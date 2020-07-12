#ifndef _DEBUG_UART_LOGGER_HPP
#define _DEBUG_UART_LOGGER_HPP

#include "serial.hpp"
#include "../std/logger_backend.hpp"

class uart_logger : public logger_backend {
public:
    uart_logger(uart& in_uart) : _uart(in_uart) {}

    void debug(const text& in_msg) override;
    void error(const text& in_msg) override;
    void info(const text& in_msg) override;
    void panic(const text& in_msg) override;
    void warn(const text& in_msg) override;

private:
    uart& _uart;
};

#endif // _DEBUG_UART_LOGGER_HPP
