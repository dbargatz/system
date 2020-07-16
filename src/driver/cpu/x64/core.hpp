#ifndef _CORE_HPP
#define _CORE_HPP

#include "std/logger.hpp"
#include "interrupts/gdt.hpp"
#include "interrupts/idt.hpp"
#include "interrupts/pic.hpp"
#include "interrupts/tss.hpp"
#include "timer/timer.hpp"
#include "keyboard/keyboard.hpp"
#include "keyboard/ps2_controller.hpp"

class core {
private:
    ///< If bit 9 is set in CPUID:01H:EDX, a local APIC is present on this core.
    static const uint32_t CPUID_01_EDX_LOCAL_APIC_PRESENT = (1 << 9);

    ///< If bit 9 (IF) is set in RFLAGS, interrupts are enabled on this core.
    static const uint64_t RFLAGS_INTERRUPTS_ENABLED_BIT   = (1 << 9);

    logger& _log;
    gdt& _gdt;
    idt& _idt;
    pic& _pic;
    timer& _timer;
    tss& _tss;
    ps2_controller& _ps2;
    keyboard& _kbd;

    bool _disable_interrupts();
    bool _enable_interrupts();
    bool _interrupts_enabled();

public:

    core(logger& in_log,
        gdt& in_gdt,
        idt& in_idt,
        pic& in_pic,
        timer& in_timer,
        tss& in_tss,
        ps2_controller& in_ps2,
        keyboard& in_kbd);

    void dispatch_interrupt(const void * in_frame_ptr);
    void panic_handler(interrupt_frame& in_frame);
    void run(const void * in_boot_info);
};

#endif // _CORE_HPP
