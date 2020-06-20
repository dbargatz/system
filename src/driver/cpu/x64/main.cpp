#include "std/logger.hpp"
#include "std/panic.h"
#include "debug/serial.hpp"
#include "interrupts/interrupt_manager.hpp"
#include "core.hpp"
#include "timer/pit.hpp"
#include "keyboard/ps2_controller.hpp"
#include "keyboard/at_keyboard.hpp"

Core * this_core;

/**
 * @brief Loops the given number of times (in millions of loops) as a rough
 * delay mechanism.
 * 
 * @param in_megaloops millions of loops to perform; if given value is 25, then
 * 25,000,000 loops will be performed
 */
void delay(uint16_t in_megaloops) {
    uint64_t loops = in_megaloops * 1000000;
    while(loops-- > 0) {
        // Loop for a bit
    }
}

extern "C" int kmain(const void * in_boot_info) {
    SerialPort serial;
    logger log(serial);
    IDT idt(log);
    PIC pic(log);
    InterruptManager intmgr(log, idt, pic);
    PIT pit(log);

    ps2_controller ps2(log);
    ps2_port kbd_port = ps2_port::INVALID;
    ps2_device_type port1 = ps2.get_type(ps2_port::PORT1);
    ps2_device_type port2 = ps2.get_type(ps2_port::PORT2);
    log.debug("PS/2 port 1: {}\n", ps2.get_type_str(port1));
    log.debug("PS/2 port 2: {}\n", ps2.get_type_str(port2));

    if(port1 == ps2_device_type::KEYBOARD_STANDARD) {
        kbd_port = ps2_port::PORT1;
    } else if(port2 == ps2_device_type::KEYBOARD_STANDARD) {
        kbd_port = ps2_port::PORT2;
    }
    at_keyboard kbd(log, ps2, kbd_port);

    Core bootstrap_core(log, in_boot_info, intmgr, pit, kbd);
    this_core = &bootstrap_core;

    bootstrap_core.run();

    PANIC("End of kmain reached!");
    return -1;
}
