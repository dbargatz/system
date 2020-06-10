#include "interrupt_manager.hpp"
#include "../std/cpuid.h"

// TODO: Handling logging this way is gross. Fix logging once migration complete.
#include "../std/logger.hpp"
extern logger gLog;

InterruptManager::InterruptManager() {
    gLog.debug("Constructing InterruptManager...\n");
    disable_interrupts();

    // Load the populated IDT into the core.
    idt.install();

    // Indicate whether a local APIC is present, just for diagnostics.
    uint32_t eax, edx;
    cpuid(1, &eax, &edx);
    gLog.debug("This core {} a local APIC.\n",
        (edx & CPUID_01_EDX_LOCAL_APIC_PRESENT) ? "has" : "doesn't have");

    // Remap IRQ 0-7 to IDT vectors 0x20-0x27 (32-39), and IRQ 8-15 to IDT
    // vectors 0x28-0x2F (40-47) to avoid conflicting with the Intel-reserved
    // range of interrupts/exceptions from 0x00-0x1F (0-31).
    pic.remap(0x20, 0x28);

    // Mask all IRQs (0-15).
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

bool InterruptManager::enabled() {
    uint64_t rflags;
    asm volatile("pushf; pop %0" : "=g"(rflags));
    return (rflags & RFLAGS_INTERRUPTS_ENABLED_BIT);
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
