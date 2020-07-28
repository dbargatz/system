#ifndef _CORE_HPP
#define _CORE_HPP

#include <cstdint.hpp>
#include "../../../logging/logger.hpp"

#include "interrupts/gdt.hpp"
#include "interrupts/idt.hpp"
#include "interrupts/pic.hpp"
#include "interrupts/tss.hpp"
#include "keyboard/keyboard.hpp"
#include "keyboard/ps2_controller.hpp"
#include "multiboot/boot_info.hpp"
#include "timer/timer.hpp"

class core {
private:
    ///< If bit 9 is set in CPUID:01H:EDX, a local APIC is present on this core.
    static const std::uint32_t CPUID_01_EDX_LOCAL_APIC_PRESENT = (1 << 9);

    ///< If bit 9 (IF) is set in RFLAGS, interrupts are enabled on this core.
    static const std::uint64_t RFLAGS_INTERRUPTS_ENABLED_BIT   = (1 << 9);

    logging::logger& _log;
    boot_info& _boot;
    gdt& _gdt;
    idt& _idt;
    keyboard& _kbd;
    pic& _pic;
    ps2_controller& _ps2;
    timer& _timer;
    tss& _tss;

    bool _disable_interrupts();
    bool _enable_interrupts();
    bool _interrupts_enabled();

public:

    core(logging::logger& in_log,
        boot_info& in_boot,
        gdt& in_gdt,
        idt& in_idt,
        keyboard& in_kbd,
        pic& in_pic,
        ps2_controller& in_ps2,
        timer& in_timer,
        tss& in_tss);

    void dispatch_interrupt(const void * in_frame_ptr);
    void panic_handler(interrupt_frame& in_frame);
    void run(const void * in_boot_info);
};

#endif // _CORE_HPP
