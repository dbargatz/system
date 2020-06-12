#include "interrupt_manager.hpp"
#include "../std/cpuid.h"
#include "../std/halt.h"
#include "../std/panic.h"

extern interrupt_handler_t interrupt_handler_0;
extern interrupt_handler_t interrupt_handler_6;

void dump_interrupt_frame(logger& in_log, logger::level in_level, struct interrupt_frame * in_frame) {
    in_log.write(in_level, "Registers:\n");
    in_log.write(in_level, "\tcs : {#016X}\trip: {#016X}\tss : {#016X}\trsp: {#016X}\n",
                 in_frame->cs, in_frame->rip, in_frame->ss, in_frame->rsp);
    in_log.write(in_level, "\trax: {#016X}\trbx: {#016X}\trcx: {#016X}\trdx: {#016X}\n",
                 in_frame->rax, in_frame->rbx, in_frame->rcx, in_frame->rdx);
    in_log.write(in_level, "\trdi: {#016X}\trsi: {#016X}\tr8 : {#016X}\tr9 : {#016X}\n",
                 in_frame->rdi, in_frame->rsi, in_frame->r8, in_frame->r9);
    in_log.write(in_level, "\tr10: {#016X}\tr11: {#016X}\tr12: {#016X}\tr13: {#016X}\n",
                 in_frame->r10, in_frame->r11, in_frame->r12, in_frame->r13);
    in_log.write(in_level, "\tr14: {#016X}\tr15: {#016X}\n", in_frame->r14, in_frame->r15);
    in_log.write(in_level, "\trflags: {#016X}\n", in_frame->rflags);
}

extern "C" void panic_handler(logger& in_log, struct interrupt_frame * in_frame) {
    struct panic_data * data = (struct panic_data *)in_frame->rip;

    in_log.panic("\n");
    in_log.panic("\n");
    in_log.panic("PANIC({}:{}): {}\n", data->filename, data->lineNum, data->msg);
    dump_interrupt_frame(in_log, logger::level::Panic, in_frame);
    in_log.panic("Raw panic data:\n");
    in_log.hexdump(logger::level::Panic, (void *)data, sizeof(*data), 1);
    in_log.panic("\n");
    in_log.panic("\n");
}

extern "C" void dispatch_interrupt(struct interrupt_frame * regs) {
    SerialPort serial;
    logger log(serial);

    switch(regs->interrupt_number) {
        case 6:
            panic_handler(log, regs);
            break;
        default:
            log.panic("UNHANDLED INTERRUPT {}!\n", regs->interrupt_number);
            dump_interrupt_frame(log, logger::level::Panic, regs);
            break;
    }

    log.warn("Halting!\n");
    halt();
}

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

    _idt.register_handler(0, &interrupt_handler_0);
    _idt.register_handler(6, &interrupt_handler_6);

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

void InterruptManager::register_handler(InterruptType in_interrupt, interrupt_handler_t * in_handler) {
    // If interrupts are enabled, disable them.
    bool were_enabled = enabled();
    if(were_enabled) {
        disable_interrupts();
    }

    // Register the handler and, if it's a PIC-handled interrupt (IRQ 0-15),
    // enable it in the PIC.
    _idt.register_handler((uint8_t)in_interrupt, in_handler);
    switch(in_interrupt) {
        case InterruptType::TIMER_EXPIRED:
            _pic.enable_irq(0);
            break;
        default:
            break;
    }

    // Re-enable interrupts if they were enabled on entry.
    if(were_enabled) {
        enable_interrupts();
    }
}
