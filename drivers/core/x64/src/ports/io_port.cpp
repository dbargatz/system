#include "io_port.hpp"

void core::x64::ports::io_port::outb(const std::uint8_t in_data) const {
    asm volatile("outb %0, %1" : /* No outputs */
                               : "a"(in_data), "Nd"(_port)
                               : /* No clobbers */);
}

void core::x64::ports::io_port::outw(const std::uint16_t in_data) const {
    asm volatile("outw %0, %1" : /* No outputs */
                               : "a"(in_data), "Nd"(_port)
                               : /* No clobbers */);
}

void core::x64::ports::io_port::outl(const std::uint32_t in_data) const {
    asm volatile("outl %0, %1" : /* No outputs */
                               : "a"(in_data), "Nd"(_port)
                               : /* No clobbers */);
}

std::uint8_t core::x64::ports::io_port::inb() const {
    std::uint8_t retval;
    asm volatile("inb %1, %0" : "=a"(retval)
                              : "Nd"(_port)
                              : /* No clobbers */);
    return retval;
}

std::uint16_t core::x64::ports::io_port::inw() const {
    std::uint16_t retval;
    asm volatile("inw %1, %0" : "=a"(retval)
                              : "Nd"(_port)
                              : /* No clobbers */);
    return retval;
}

std::uint32_t core::x64::ports::io_port::inl() const {
    std::uint32_t retval;
    asm volatile("inl %1, %0" : "=a"(retval)
                              : "Nd"(_port)
                              : /* No clobbers */);
    return retval;
}
