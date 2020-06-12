#ifndef _CORE_HPP
#define _CORE_HPP

#include "debug/serial.hpp"
#include "std/stdint.h"
#include "std/logger.hpp"
#include "interrupts/interrupt_manager.hpp"

class Core {
private:
    logger& _log;
    const void * _boot_info;
    InterruptManager& _interrupts;
    SerialPort& _serial;

public:
    Core(logger& in_log,
        const void * in_boot_info,
        InterruptManager& in_interrupts,
        SerialPort& in_serial);
};

#endif // _CORE_HPP
