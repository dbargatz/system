#ifndef _CORE_HPP
#define _CORE_HPP

#include "std/logger.hpp"
#include "interrupts/interrupt_manager.hpp"
#include "timer/pit.hpp"

class Core {
private:
    logger& _log;
    const void * _boot_info;
    InterruptManager& _interrupts;
    ITimer& _timer;

public:
    Core(logger& in_log,
        const void * in_boot_info,
        InterruptManager& in_interrupts,
        ITimer& in_timer);

    void run();
};

#endif // _CORE_HPP
