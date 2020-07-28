#include "io_port.hpp"

void io_port::outb(const std::uint8_t in_data) const {
    asm volatile("outb %0, %1" : /* No outputs */
                               : "a"(in_data), "Nd"(_port)
                               : /* No clobbers */);
}

void io_port::outw(const std::uint16_t in_data) const {
    asm volatile("outw %0, %1" : /* No outputs */
                               : "a"(in_data), "Nd"(_port)
                               : /* No clobbers */);
}

void io_port::outl(const std::uint32_t in_data) const {
    asm volatile("outl %0, %1" : /* No outputs */
                               : "a"(in_data), "Nd"(_port)
                               : /* No clobbers */);
}

std::uint8_t io_port::inb() const {
    std::uint8_t retval;
    asm volatile("inb %1, %0" : "=a"(retval)
                              : "Nd"(_port)
                              : /* No clobbers */);
    return retval;
}

std::uint16_t io_port::inw() const {
    std::uint16_t retval;
    asm volatile("inw %1, %0" : "=a"(retval)
                              : "Nd"(_port)
                              : /* No clobbers */);
    return retval;
}

std::uint32_t io_port::inl() const {
    std::uint32_t retval;
    asm volatile("inl %1, %0" : "=a"(retval)
                              : "Nd"(_port)
                              : /* No clobbers */);
    return retval;
}
