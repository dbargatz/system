#ifndef _INTERRUPTS_PIC_HPP
#define _INTERRUPTS_PIC_HPP

#include <cstdint>
#include "../../../../logging/logger.hpp"
#include "../ports/io_port.hpp"

class pic {
private:
    constexpr static const io_port PIC1_COMMAND_PORT = io_port(0x0020);
    constexpr static const io_port PIC1_DATA_PORT    = io_port(0x0021);
    constexpr static const io_port PIC2_COMMAND_PORT = io_port(0x00A0);
    constexpr static const io_port PIC2_DATA_PORT    = io_port(0x00A1);

    constexpr static const std::uint8_t EOI_COMMAND  = 0x20;
    constexpr static const std::uint8_t INIT_COMMAND = 0x11;

    constexpr static const std::uint8_t PIC2_IRQ_NUMBER = 0x02;

    constexpr static const std::uint8_t CPU_MODE_8086 = 0x01;

    logging::logger& _log;

public:
    /**
     * @brief Construct a new Programmable Interrupt Controller (PIC) object.
     * 
     * @param in_log logger for member functions
     */
    pic(logging::logger& in_log);

    void send_eoi(const std::uint8_t in_irq_number);
    void remap(const std::uint8_t in_pic1_interrupt_base,
               const std::uint8_t in_pic2_interrupt_base);
    void disable_all(void);
    void disable_irq(const std::uint8_t in_irq_number);
    void enable_irq(const std::uint8_t in_irq_number);
};

#endif // _INTERRUPTS_PIC_HPP
