#include "interrupt_manager.hpp"
#include "../std/cpuid.h"

InterruptManager::InterruptManager(logger& in_log, IDT& in_idt, PIC& in_pic) : 
    _log(in_log), _idt(in_idt), _pic(in_pic) {
    _log.debug("Constructing InterruptManager...\n");
    disable_interrupts();

    // Load the populated IDT into the core.
    _idt.install();

    // Indicate whether a local APIC is present, just for diagnostics.
    uint32_t eax, edx;
    cpuid(1, &eax, &edx);
    _log.debug("This core {} a local APIC.\n",
        (edx & CPUID_01_EDX_LOCAL_APIC_PRESENT) ? "has" : "doesn't have");

    // Remap IRQ 0-7 to IDT vectors 0x20-0x27 (32-39), and IRQ 8-15 to IDT
    // vectors 0x28-0x2F (40-47) to avoid conflicting with the Intel-reserved
    // range of interrupts/exceptions from 0x00-0x1F (0-31).
    _pic.remap(0x20, 0x28);

    // Mask all IRQs (0-15).
    _pic.disable_all();

    _log.debug("Constructed InterruptManager.\n");
}

void InterruptManager::disable_interrupts() {
    asm volatile("cli");
    _log.debug("Interrupts disabled.\n");
}

void InterruptManager::enable_interrupts() {
    asm volatile("sti");
    _log.debug("Interrupts enabled.\n");
}

bool InterruptManager::enabled() {
    uint64_t rflags;
    asm volatile("pushf; pop %0" : "=g"(rflags));
    return (rflags & RFLAGS_INTERRUPTS_ENABLED_BIT);
}

void InterruptManager::handler_complete(InterruptType in_interrupt) {
    switch(in_interrupt) {
        case InterruptType::TIMER_EXPIRED:
            _pic.send_eoi(0);
            break;
        default:
            _log.warn("Cannot mark handler complete for interrupt {}\n", (uint8_t)in_interrupt);
            break;
    }
}

void InterruptManager::register_handler(InterruptType in_interrupt, void (*in_handler)(struct interrupt_frame *)) {
    disable_interrupts();
    _idt.register_handler((uint8_t)in_interrupt, in_handler);
    switch(in_interrupt) {
        case InterruptType::TIMER_EXPIRED:
            _pic.enable_irq(0);
            break;
        default:
            break;
    }
    enable_interrupts();
}
