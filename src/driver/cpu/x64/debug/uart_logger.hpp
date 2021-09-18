#ifndef _DEBUG_UART_LOGGER_HPP
#define _DEBUG_UART_LOGGER_HPP

#include <string>
#include "../../../../lib/libsystem/logger.hpp"
#include "uart.hpp"

class uart_backend : public logging::backend {
public:
    uart_backend(uart& in_uart) : _uart(in_uart) {}

    void write(const std::string& in_str) override;

private:
    uart& _uart;
};

#endif // _DEBUG_UART_LOGGER_HPP
