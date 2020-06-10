#ifndef _PORTS_ASM_H
#define _PORTS_ASM_H

#include "../std/stdint.h"

/**
 * @brief Read a byte from the given I/O port address.
 * 
 * @param in_io_port I/O port to read from
 * @return uint8_t Byte that was read
 */
static inline uint8_t inb(const uint16_t in_io_port) {
    uint8_t retval;
    asm volatile("inb %1, %0" : "=a"(retval) : "Nd"(in_io_port));
    return retval;
}

/**
 * @brief Write the given byte to the given I/O port address.
 * 
 * @param in_io_port I/O port to write to
 * @param in_byte Byte to write
 */
static inline void outb(const uint16_t in_io_port, const uint8_t in_byte) {
    asm volatile("outb %0, %1" : : "a"(in_byte), "Nd"(in_io_port));
}

#endif // _PORTS_ASM_H