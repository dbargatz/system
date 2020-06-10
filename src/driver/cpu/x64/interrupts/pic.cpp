#include "pic.hpp"
#include "../ports/asm.h"

// TODO: Handling logging this way is gross. Fix logging once migration complete.
#include "../std/logger.hpp"
extern logger gLog;

PIC::PIC() {
    gLog.debug("Initialized PIC.\n");
}

void PIC::send_eoi(const uint8_t in_irq_number) {
    // TODO: assert 0 <= in_irq_number < 15

    if(in_irq_number >= 8) {
        outb(PIC2_COMMAND_PORT, EOI_COMMAND);
    }
    outb(PIC1_COMMAND_PORT, EOI_COMMAND);
}

void PIC::remap(const uint8_t in_pic1_interrupt_base,
                const uint8_t in_pic2_interrupt_base) {

    // Save off the current interrupt masks.
    uint8_t pic1_mask = inb(PIC1_DATA_PORT);
    uint8_t pic2_mask = inb(PIC2_DATA_PORT);

    // Start the initialization of both PICs. After this command is sent,
    // each PIC will expect three data bytes in sequence on their data port:
    //     1. ICW2: The base interrupt vector to deliver IRQs on for this PIC.
    //     2. ICW3: The cascaded identity of the cascaded PICs (if any)
    //     3. ICW4: The CPU mode to deliver interrupts as
    // See Figures 14.75 - 14.79 in the 8259 PIC datasheet for more.
    outb(PIC1_COMMAND_PORT, INIT_COMMAND);
    outb(PIC2_COMMAND_PORT, INIT_COMMAND);

    // Send ICW2, the interrupt vector base.
    outb(PIC1_DATA_PORT, in_pic1_interrupt_base);
    outb(PIC2_DATA_PORT, in_pic2_interrupt_base);

    // Send ICW3, the cascaded PIC identity.
    outb(PIC1_DATA_PORT, PIC2_IRQ_NUMBER << 2);
    outb(PIC2_DATA_PORT, PIC2_IRQ_NUMBER);

    // Send ICW4, and set the delivery mode as 8086/8088.
    outb(PIC1_DATA_PORT, CPU_MODE_8086);
    outb(PIC2_DATA_PORT, CPU_MODE_8086);

    // Restore the saved interrupt masks.
    // TODO: Is this necessary? We don't change the mask above, does init wipe out the mask? Check datasheet.
    outb(PIC1_DATA_PORT, pic1_mask);
    outb(PIC2_DATA_PORT, pic2_mask);
}

void PIC::disable_all(void) {
    // Mask all interrupts on both the PICs.
    outb(PIC2_DATA_PORT, 0xFF);
    outb(PIC1_DATA_PORT, 0xFF);
    gLog.debug("Disabled all IRQs\n");
}

void PIC::disable_irq(const uint8_t in_irq_number) {
    // TODO: assert 0 <= in_irq_number < 15
    uint8_t mask;

    if(in_irq_number < 8) {
        mask = inb(PIC1_DATA_PORT) | (1 << in_irq_number);
        outb(PIC1_DATA_PORT, mask);
    } else {
        mask = inb(PIC2_DATA_PORT) | (1 << (in_irq_number - 8));
        outb(PIC2_DATA_PORT, mask);
    }
    gLog.debug("Disabled IRQ {#02X} ({})\n", in_irq_number, in_irq_number);
}

void PIC::enable_irq(const uint8_t in_irq_number) {
    // TODO: assert 0 <= in_irq_number < 15
    uint8_t mask;

    if(in_irq_number < 8) {
        mask = inb(PIC1_DATA_PORT) & ~(1 << in_irq_number);
        outb(PIC1_DATA_PORT, mask);
    } else {
        mask = inb(PIC2_DATA_PORT) & ~(1 << (in_irq_number - 8));
        outb(PIC2_DATA_PORT, mask);
    }
    gLog.debug("Enabled IRQ {#02X} ({})\n", in_irq_number, in_irq_number);
}
