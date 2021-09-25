#ifndef _CORE_X64_LOGGING_UART_LOGGER_HPP
#define _CORE_X64_LOGGING_UART_LOGGER_HPP

#include <string>
#include "logger.hpp"
#include "uart.hpp"

namespace core::x64::logging {

class uart_backend : public backend {
public:
    uart_backend(uart& in_uart) : _uart(in_uart) {}

    void write(const std::string& in_str) override;

private:
    uart& _uart;
};

}; // namespace core::x64::logging

#endif // _CORE_X64_LOGGING_UART_LOGGER_HPP
