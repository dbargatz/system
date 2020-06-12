#include "core.hpp"
#include "std/halt.h"
#include "std/panic.h"

Core::Core(logger& in_log, const void * in_boot_info,
    InterruptManager& in_interrupts) : 
    _log(in_log), _boot_info(in_boot_info), _interrupts(in_interrupts) {
        // Register the default panic handler.
        //_interrupts.register_handler(InterruptType::PANIC, panic_handler);

        _log.debug("Constructed Core:\n");
        _log.debug("    Boot Info    : {#016X}\n", (uint64_t)_boot_info);
        _log.debug("    Interrupts   : {}abled\n", _interrupts.enabled() ? "en" : "dis");
        //_log.debug("    Panic Handler: {#016X}\n", (uint64_t)panic_handler);
        _log.debug("    Serial Port  : present\n");
}

void Core::run() {
    _log.info("Halting core, waiting for interrupts...\n");
    PANIC("oh bother");
    halt();
}
