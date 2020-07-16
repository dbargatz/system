#include "uart_logger.hpp"

void uart_logger::debug(const text& in_msg) {
    _uart.write(in_msg.get());
}

void uart_logger::error(const text& in_msg) {
    _uart.write(in_msg.get());
}

void uart_logger::info(const text& in_msg) {
    _uart.write(in_msg.get());
}

void uart_logger::panic(const text& in_msg) {
    _uart.write(in_msg.get());
}

void uart_logger::warn(const text& in_msg) {
    _uart.write(in_msg.get());
}
