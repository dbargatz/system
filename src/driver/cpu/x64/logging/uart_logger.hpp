#ifndef _CPU_X64_LOGGING_UART_LOGGER_HPP
#define _CPU_X64_LOGGING_UART_LOGGER_HPP

#include <string>
#include "logger.hpp"
#include "uart.hpp"

class uart_backend : public cpu::x64::logging::backend {
public:
    uart_backend(uart& in_uart) : _uart(in_uart) {}

    void write(const std::string& in_str) override;

private:
    uart& _uart;
};

#endif // _CPU_X64_LOGGING_UART_LOGGER_HPP
