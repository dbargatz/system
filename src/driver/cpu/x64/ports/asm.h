#ifndef _PORTS_ASM_H
#define _PORTS_ASM_H

#include "../std/stdint.h"
#include "io.h"

/**
 * @brief Read a byte from the given I/O port address.
 * 
 * @param in_addr I/O port address to read from
 * @return uint8_t Byte that was read
 */
static inline uint8_t inb(const io_port_addr_t in_addr) {
    uint8_t retval;
    asm volatile("inb %1, %0" : "=a"(retval) : "Nd"(in_addr));
    return retval;
}

/**
 * @brief Write the given byte to the given I/O port address.
 * 
 * @param in_addr I/O port address to write to
 * @param in_byte Byte to write
 */
static inline void outb(const io_port_addr_t in_addr, const uint8_t in_byte) {
    asm volatile("outb %0, %1" : : "a"(in_byte), "Nd"(in_addr));
}

#endif // _PORTS_ASM_H
