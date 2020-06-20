#ifndef _CORE_HPP
#define _CORE_HPP

#include "std/logger.hpp"
#include "interrupts/interrupt_manager.hpp"
#include "timer/ITimer.hpp"
#include "keyboard/keyboard.hpp"

class Core {
private:
    logger& _log;
    keyboard& _kbd;
    const void * _boot_info;

public:
    ITimer& timer;
    InterruptManager& interrupts;

    Core(logger& in_log,
        const void * in_boot_info,
        InterruptManager& in_interrupts,
        ITimer& in_timer,
        keyboard& in_kbd);

    void run();
};

#endif // _CORE_HPP
