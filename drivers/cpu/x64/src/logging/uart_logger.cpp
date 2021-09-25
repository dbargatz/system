#include "uart_logger.hpp"

void uart_backend::write(const std::string& in_str) {
    _uart.write(in_str);
}
