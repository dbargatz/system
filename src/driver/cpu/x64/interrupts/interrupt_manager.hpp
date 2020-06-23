#ifndef _INTERRUPTS_INTERRUPT_MANAGER_HPP
#define _INTERRUPTS_INTERRUPT_MANAGER_HPP

#include "frame.hpp"
#include "idt.hpp"
#include "pic.hpp"
#include "../std/logger.hpp"

enum class InterruptType : uint8_t {
    PANIC         = 6,
    TIMER_EXPIRED = 32,
    KEYBOARD      = 33,
};

typedef void(*interrupt_handler)(logger& in_log, interrupt_frame& in_frame);

class InterruptManager {
private:
    ///< If bit 9 is set in CPUID:01H:EDX, a local APIC is present on this core.
    static const uint32_t CPUID_01_EDX_LOCAL_APIC_PRESENT = (1 << 9);
    static const uint64_t RFLAGS_INTERRUPTS_ENABLED_BIT   = (1 << 9);

    logger& _log;
    IDT& _idt;
    PIC& _pic;

public:
    /**
     * @brief Construct a new Interrupt Manager object.
     * 
     * @param in_log logger for member functions
     * @param in_idt x86_64 Interrupt Descriptor Table (IDT)
     * @param in_pic 8259 Programmable Interrupt Controller (PIC)
     */
    InterruptManager(logger& in_log, IDT& in_idt, PIC& in_pic);

    /**
     * @brief Disable interrupts on this core (excluding NMIs).
     */
    void disable_interrupts();

    /**
     * @brief Enable interrupts on this core.
     */
    void enable_interrupts();

    /**
     * @brief Returns whether or not interrupts are enabled on this core.
     * 
     * @return true interrupts are enabled (IF = 1 in RFLAGS)
     * @return false interrupts are enabled (IF = 0 in RFLAGS)
     */
    bool enabled();

    void handler_complete(InterruptType in_interrupt);

    void register_handler(InterruptType in_interrupt, const interrupt_handler in_handler);

    void temp_unmask(uint8_t in_irq) { _pic.enable_irq(in_irq); }
};

#endif // _INTERRUPTS_INTERRUPT_MANAGER_HPP
