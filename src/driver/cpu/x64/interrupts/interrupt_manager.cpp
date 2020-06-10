#include "interrupt_manager.hpp"

// TODO: Handling logging this way is gross. Fix logging once migration complete.
#include "../std/logger.hpp"
extern kernel::platform::x86_64::logger gLog;

InterruptManager::InterruptManager() {
    gLog.debug("Constructing InterruptManager...\n");
    disable_interrupts();

    // Load the populated IDT into the core.
    idt.install();

    pic.remap(0x20, 0x28);
    pic.disable_all();

    gLog.debug("Constructed InterruptManager.\n");
}

void InterruptManager::disable_interrupts() {
    asm volatile("cli");
    gLog.debug("Interrupts disabled.\n");
}

void InterruptManager::enable_interrupts() {
    asm volatile("sti");
    gLog.debug("Interrupts enabled.\n");
}

void InterruptManager::handler_complete(InterruptType in_interrupt) {
    switch(in_interrupt) {
        case InterruptType::TIMER_EXPIRED:
            pic.send_eoi(0);
            break;
        default:
            gLog.warn("Cannot mark handler complete for interrupt {}\n", (uint8_t)in_interrupt);
            break;
    }
}

void InterruptManager::register_handler(InterruptType in_interrupt, void (*in_handler)(struct interrupt_frame *)) {
    disable_interrupts();
    idt.register_handler((uint8_t)in_interrupt, in_handler);
    switch(in_interrupt) {
        case InterruptType::TIMER_EXPIRED:
            pic.enable_irq(0);
            break;
        default:
            break;
    }
    enable_interrupts();
}
