#ifndef _CORE_ARM64_LOGGING_UART_HPP
#define _CORE_ARM64_LOGGING_UART_HPP

namespace core::arm64::console {

class uart {
public:
    uart();

    void write(const char* in_str);
};

}; // namespace core::arm64::console

#endif // _CORE_ARM64_LOGGING_UART_HPP
