#include "core.hpp"
#include "std/assert.h"

Core::Core(logger& in_log, gdt& in_gdt, tss& in_tss, const void * in_boot_info,
    InterruptManager& in_interrupts, ITimer& in_timer, keyboard& in_kbd) :
     _gdt(in_gdt), _tss(in_tss), _log(in_log), _boot_info(in_boot_info), timer(in_timer),
     interrupts(in_interrupts), kbd(in_kbd) {
        _log.debug("Constructed Core:");
        _log.debug("    Boot Info    : {#016X}", (uint64_t)_boot_info);
        _log.debug("    Interrupts   : {}abled", interrupts.enabled() ? "en" : "dis");
        _log.debug("    PIT          : present");
        _log.debug("    Serial Port  : present");
        _log.debug("    Keyboard     : present");
        _gdt.dump();
        _tss.dump();
}

void Core::run() {
    kbd.reset();
    interrupts.enable_interrupts();
    interrupts.temp_unmask(1);
    _log.info("Testing keyboard (interrupts: {}abled)...", interrupts.enabled() ? "en" : "dis");
    uint32_t ct = 0xFFFFFFFF;
    while(ct > 0) {
        ct--;
    }

    timer.set_frequency(1000.0);
    interrupts.temp_unmask(0);
    _log.info("Testing timer (interrupts: {}abled)...", interrupts.enabled() ? "en" : "dis");
    ct = 0xFFFFFFFF;
    while(ct > 0) {
        ct--;
    }

    _log.info("Testing ASSERT and PANIC (will halt)...");
     ASSERT(0 == 1, "oh bother");
}
