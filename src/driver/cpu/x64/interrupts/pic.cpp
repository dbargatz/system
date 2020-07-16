#include "pic.hpp"

pic::pic(logger& in_log) : _log(in_log) {
    _log.debug("Initialized PIC.");
}

void pic::send_eoi(const uint8_t in_irq_number) {
    // TODO: assert 0 <= in_irq_number < 15

    if(in_irq_number >= 8) {
        PIC2_COMMAND_PORT.outb(EOI_COMMAND);
    }
    PIC1_COMMAND_PORT.outb(EOI_COMMAND);
}

void pic::remap(const uint8_t in_pic1_interrupt_base,
                const uint8_t in_pic2_interrupt_base) {

    // Save off the current interrupt masks.
    uint8_t pic1_mask = PIC1_DATA_PORT.inb();
    uint8_t pic2_mask = PIC2_DATA_PORT.inb();

    // Start the initialization of both PICs. After this command is sent,
    // each PIC will expect three data bytes in sequence on their data port:
    //     1. ICW2: The base interrupt vector to deliver IRQs on for this PIC.
    //     2. ICW3: The cascaded identity of the cascaded PICs (if any)
    //     3. ICW4: The CPU mode to deliver interrupts as
    // See Figures 14.75 - 14.79 in the 8259 PIC datasheet for more.
    PIC1_COMMAND_PORT.outb(INIT_COMMAND);
    PIC2_COMMAND_PORT.outb(INIT_COMMAND);

    // Send ICW2, the interrupt vector base.
    PIC1_DATA_PORT.outb(in_pic1_interrupt_base);
    PIC2_DATA_PORT.outb(in_pic2_interrupt_base);

    // Send ICW3, the cascaded PIC identity.
    PIC1_DATA_PORT.outb(PIC2_IRQ_NUMBER << 2);
    PIC2_DATA_PORT.outb(PIC2_IRQ_NUMBER);

    // Send ICW4, and set the delivery mode as 8086/8088.
    PIC1_DATA_PORT.outb(CPU_MODE_8086);
    PIC2_DATA_PORT.outb(CPU_MODE_8086);

    // Restore the saved interrupt masks.
    // TODO: Is this necessary? We don't change the mask above, does init wipe out the mask? Check datasheet.
    PIC1_DATA_PORT.outb(pic1_mask);
    PIC2_DATA_PORT.outb(pic2_mask);
}

void pic::disable_all(void) {
    // Mask all interrupts on both the PICs.
    PIC2_DATA_PORT.outb(0xFF);
    PIC1_DATA_PORT.outb(0xFF);
    _log.debug("Disabled all IRQ");
}

void pic::disable_irq(const uint8_t in_irq_number) {
    // TODO: assert 0 <= in_irq_number < 15
    uint8_t mask;

    if(in_irq_number < 8) {
        mask = PIC1_DATA_PORT.inb() | (1 << in_irq_number);
        PIC1_DATA_PORT.outb(mask);
    } else {
        mask = PIC2_DATA_PORT.inb() | (1 << (in_irq_number - 8));
        PIC2_DATA_PORT.outb(mask);
    }
    _log.debug("Disabled IRQ {#02X} ({})", in_irq_number, in_irq_number);
}

void pic::enable_irq(const uint8_t in_irq_number) {
    // TODO: assert 0 <= in_irq_number < 15
    uint8_t mask;

    if(in_irq_number < 8) {
        mask = PIC1_DATA_PORT.inb() & ~(1 << in_irq_number);
        PIC1_DATA_PORT.outb(mask);
    } else {
        mask = PIC2_DATA_PORT.inb() & ~(1 << (in_irq_number - 8));
        PIC2_DATA_PORT.outb(mask);
    }
    _log.debug("Enabled IRQ {#02X} ({})", in_irq_number, in_irq_number);
}
