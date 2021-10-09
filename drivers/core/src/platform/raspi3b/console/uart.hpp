#ifndef _CORE_PLATFORM_RASPI3B_CONSOLE_UART_HPP
#define _CORE_PLATFORM_RASPI3B_CONSOLE_UART_HPP

namespace core::console {

class uart {
public:
    uart();

    void write(const char* in_str);
};

}; // namespace core::console

#endif // _CORE_PLATFORM_RASPI3B_CONSOLE_UART_HPP
