#include "core.hpp"

Core::Core(logger& in_log, const void * in_boot_info,
    InterruptManager& in_interrupts,
    SerialPort& in_serial) : 
    _log(in_log), _boot_info(in_boot_info), _interrupts(in_interrupts), 
    _serial(in_serial) {
        _log.debug("Constructed Core:\n");
        _log.debug("    Boot Info  : {#016X}\n", (uint64_t)_boot_info);
        _log.debug("    Interrupts : {}abled\n", _interrupts.enabled() ? "en" : "dis");
        _log.debug("    Serial Port: present\n");
        _serial.write(text("Hello serial world!\n"));
}
