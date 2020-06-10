#ifndef _INTERRUPTS_INTERRUPT_MANAGER_HPP
#define _INTERRUPTS_INTERRUPT_MANAGER_HPP

#include "idt.hpp"
#include "pic.hpp"

enum class InterruptType : uint8_t {
    PANIC         = 6,
    TIMER_EXPIRED = 32,
    KEYPRESS      = 33,
};

class InterruptManager {
private:
    IDT idt;
    PIC pic;

public:
    /**
     * @brief Construct a new Interrupt Manager object.
     */
    InterruptManager();

    /**
     * @brief Disable interrupts on this core (excluding NMIs).
     */
    void disable_interrupts();

    /**
     * @brief Enable interrupts on this core.
     */
    void enable_interrupts();

    void handler_complete(InterruptType in_interrupt);

    void register_handler(InterruptType in_interrupt, void (*in_handler)(struct interrupt_frame *));
};

#endif // _INTERRUPTS_INTERRUPT_MANAGER_HPP
