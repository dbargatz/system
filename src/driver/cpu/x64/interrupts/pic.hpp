#ifndef _PIC_HPP
#define _PIC_HPP

#include "../inline_asm.hpp"
// TODO: redirect to local stdint.h when migration complete.
#include "../../../../kernel/platform/qemu-system-x86_64/types/stdint.h"

class PIC {
private:
    static const uint16_t PIC1_COMMAND_PORT = 0x0020;
    static const uint16_t PIC1_DATA_PORT    = 0x0021;
    static const uint16_t PIC2_COMMAND_PORT = 0x00A0;
    static const uint16_t PIC2_DATA_PORT    = 0x00A1;

    static const uint8_t EOI_COMMAND  = 0x20;
    static const uint8_t INIT_COMMAND = 0x11;

    static const uint8_t PIC2_IRQ_NUMBER = 0x02;

    static const uint8_t CPU_MODE_8086 = 0x01;

public:
    PIC();

    void send_eoi(const uint8_t in_irq_number);
    void remap(const uint8_t in_pic1_interrupt_base, 
               const uint8_t in_pic2_interrupt_base);
    void disable_all(void);
    void disable_irq(const uint8_t in_irq_number);
    void enable_irq(const uint8_t in_irq_number);
};

#endif // _PIC_HPP
