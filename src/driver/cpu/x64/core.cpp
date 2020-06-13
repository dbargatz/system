#include "core.hpp"
#include "std/halt.h"
#include "std/panic.h"

Core::Core(logger& in_log, const void * in_boot_info,
    InterruptManager& in_interrupts, PIT& in_pit) : 
    _log(in_log), _boot_info(in_boot_info), _interrupts(in_interrupts), 
    _pit(in_pit) {
        _log.debug("Constructed Core:\n");
        _log.debug("    Boot Info    : {#016X}\n", (uint64_t)_boot_info);
        _log.debug("    Interrupts   : {}abled\n", _interrupts.enabled() ? "en" : "dis");
        _log.debug("    PIT          : present\n");
        _log.debug("    Serial Port  : present\n");
}

void Core::run() {
    _log.info("Validating software interrupts...\n");
    asm volatile("int $32\n");
    asm volatile("int $45\n");
    asm volatile("int $62\n");
    asm volatile("int $98\n");
    asm volatile("int $150\n");
    asm volatile("int $254\n");
    asm volatile("int $255\n");

    _log.info("Done with software interrupts, testing timer! Interrupt state: {}\n", _interrupts.enabled() ? "enabled" : "disabled");
    _pit.set_frequency(50);
    _interrupts.temp_unmask(0);
    _interrupts.enable_interrupts();
    uint32_t ct = 0xFFFFFFFF;
    while(ct > 0) {
        ct--;
    }

    _log.info("Done with timer, panicking!\n");
    PANIC("oh bother");
}
