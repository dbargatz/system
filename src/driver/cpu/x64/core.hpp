#ifndef _CORE_HPP
#define _CORE_HPP

#include "std/logger.hpp"
#include "interrupts/interrupt_manager.hpp"
#include "interrupts/tss.hpp"
#include "timer/ITimer.hpp"
#include "keyboard/keyboard.hpp"

class Core {
private:
    tss& _tss;
    logger& _log;
    const void * _boot_info;

public:
    ITimer& timer;
    InterruptManager& interrupts;
    keyboard& kbd;

    Core(logger& in_log,
        tss& in_tss,
        const void * in_boot_info,
        InterruptManager& in_interrupts,
        ITimer& in_timer,
        keyboard& in_kbd);

    void run();
};

#endif // _CORE_HPP
