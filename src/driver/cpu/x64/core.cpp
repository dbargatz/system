#include "core.hpp"
#include "std/halt.h"
#include "std/panic.h"

extern "C" __attribute__((interrupt))
void panic_handler(struct interrupt_frame * in_frame) {
    SerialPort serial;
    logger log(serial);
    struct panic_data * data = (struct panic_data *)in_frame->rip;

    log.error("PANIC({}:{}): {}\n", data->filename, data->lineNum, data->msg);
    log.hexdump(logger::level::Error, (void *)data, sizeof(*data));
    halt();
}

Core::Core(logger& in_log, const void * in_boot_info,
    InterruptManager& in_interrupts) : 
    _log(in_log), _boot_info(in_boot_info), _interrupts(in_interrupts) {
        // Register the default panic handler.
        _interrupts.register_handler(InterruptType::PANIC, panic_handler);

        _log.debug("Constructed Core:\n");
        _log.debug("    Boot Info    : {#016X}\n", (uint64_t)_boot_info);
        _log.debug("    Interrupts   : {}abled\n", _interrupts.enabled() ? "en" : "dis");
        _log.debug("    Panic Handler: {#016X}\n", (uint64_t)panic_handler);
        _log.debug("    Serial Port  : present\n");
}

void Core::run() {
    _log.info("Halting core, waiting for interrupts...\n");
    halt();
}
