#include "uart_logger.hpp"

void core::x64::logging::uart_backend::write(const std::string& in_str) {
    _uart.write(in_str);
}
