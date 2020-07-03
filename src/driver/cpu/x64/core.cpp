#include "core.hpp"
#include "std/assert.h"
#include "std/queue.hpp"

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
    interrupts.enable_interrupts();

    queue<uint8_t, 16> q;
    for(auto i = 0; i < 48; i++) {
        q.enqueue(i);
        ASSERT(q.dequeue() == i, "dequeued wrong value");
    }

    kbd.reset();
    interrupts.temp_unmask(1);
    _log.info("Testing keyboard (interrupts: {}abled)...", interrupts.enabled() ? "en" : "dis");
    uint32_t ct = 0x7FFFFFFF;
    while(ct > 0) {
        ct--;
    }

    timer.set_frequency(1000.0);
    interrupts.temp_unmask(0);
    _log.info("Testing timer (interrupts: {}abled)...", interrupts.enabled() ? "en" : "dis");
    ct = 0x1FFFFFFF;
    while(ct > 0) {
        ct--;
    }

    _log.info("Testing ASSERT and PANIC (will halt)...");
     ASSERT(0 == 1, "oh bother");
}
