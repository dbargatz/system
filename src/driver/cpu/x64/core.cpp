#include "core.hpp"
#include "std/halt.h"
#include "std/panic.h"
#include "timer/pit.hpp"

Core::Core(logger& in_log, const void * in_boot_info,
    InterruptManager& in_interrupts, ITimer& in_timer, keyboard& in_kbd) :
     _log(in_log), _boot_info(in_boot_info), timer(in_timer),
     interrupts(in_interrupts), kbd(in_kbd) {
        _log.debug("Constructed Core:\n");
        _log.debug("    Boot Info    : {#016X}\n", (uint64_t)_boot_info);
        _log.debug("    Interrupts   : {}abled\n", interrupts.enabled() ? "en" : "dis");
        _log.debug("    PIT          : present\n");
        _log.debug("    Serial Port  : present\n");
        _log.debug("    Keyboard     : present\n");
}

void Core::run() {
    kbd.reset();
    interrupts.enable_interrupts();
    interrupts.temp_unmask(1);
    _log.info("Testing keyboard (interrupts: {}abled)...\n", interrupts.enabled() ? "en" : "dis");
    uint32_t ct = 0xFFFFFFFF;
    while(ct > 0) {
        ct--;
    }

    timer.set_frequency(1000.0);
    interrupts.temp_unmask(0);
    _log.info("Testing timer (interrupts: {}abled)...\n", interrupts.enabled() ? "en" : "dis");
    ct = 0xFFFFFFFF;
    while(ct > 0) {
        ct--;
    }

    _log.info("Testing panic (will halt)...\n");
    PANIC("oh bother");
}
