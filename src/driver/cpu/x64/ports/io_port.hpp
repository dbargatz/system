#ifndef _PORTS_IO_PORT_HPP
#define _PORTS_IO_PORT_HPP

#include <cstdint.hpp>

/**
 * @brief Represents a single I/O port in the I/O address space on Intel
 * processors; primarily used for interaction with legacy hardware and
 * peripherals. Allows reading and writing of data.
 */
class io_port {
private:
    const std::uint16_t _port; ///< Address of this I/O port in I/O address space

public:
    /**
     * @brief Construct a new I/O port at the given port address.
     * 
     * @param in_port_address 16-bit address of I/O port in I/O address space
     */
    constexpr io_port(const std::uint16_t in_port_address) : _port(in_port_address) {}

    /**
     * @brief Write the given 8-bit byte to this I/O port.
     * 
     * @param in_data 8-bit byte to write
     */
    void outb(const std::uint8_t in_data) const;

    /**
     * @brief Write the given 16-bit word to this I/O port.
     * 
     * @param in_data 16-bit word to write
     */
    void outw(const std::uint16_t in_data) const;

    /**
     * @brief Write the given 32-bit long to this I/O port.
     * 
     * @param in_data 32-bit long to write
     */
    void outl(const std::uint32_t in_data) const;

    /**
     * @brief Read an 8-bit byte from this I/O port.
     * 
     * @return uint8_t 8-bit word that was read
     */
    std::uint8_t inb() const;

    /**
     * @brief Read a 16-bit word from this I/O port.
     * 
     * @return uint16_t 16-bit word that was read
     */
    std::uint16_t inw() const;

    /**
     * @brief Read a 32-bit long from this I/O port.
     * 
     * @return uint32_t 32-bit long that was read
     */
    std::uint32_t inl() const;
};

#endif // _PORTS_IO_PORT_HPP