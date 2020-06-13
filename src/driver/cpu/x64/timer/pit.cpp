#include "pit.hpp"
#include "../ports/asm.h"
#include "../std/panic.h"

float64_t freq;   // TODO: remove
float64_t uptime_ms;

extern "C" void pit_handler(logger& in_log, interrupt_frame& in_frame) {
    uptime_ms += 1000.0/freq;
    in_log.debug("Uptime: {}ms\n", (uint64_t)uptime_ms);

    // TODO: sending EOI to PIC, integrate with rest of system
    outb(0x0020, 0x20);
}

PIT::PIT(logger& in_log, uint32_t in_frequency) : _log(in_log) {
    set_frequency(in_frequency);
    _log.debug("Constructed PIT.\n");
}

void PIT::set_frequency(uint32_t in_frequency) {
    // If the frequency is too low or too high, panic.
    if(in_frequency > BASE_FREQUENCY_HZ) {
        PANIC("frequency too high");
    } else if(in_frequency < MIN_FREQUENCY_HZ) {
        PANIC("frequency too low");
    }

    // Calculate the new reload value.
    _reload_value = BASE_FREQUENCY_HZ / in_frequency;
    uint32_t remainder = BASE_FREQUENCY_HZ % in_frequency;
    if(remainder > (BASE_FREQUENCY_HZ / 2)) {
        _reload_value++;
    }

    // TODO: remove statics
    freq = (float64_t)BASE_FREQUENCY_HZ/(float64_t)_reload_value;

    // Install the new reload value in the actual hardware.
    union _command_reg cmd;
    cmd.select_channel  = 0b00;  // Channel 0
    cmd.access_mode     = 0b11;  // Read/write lobyte,hibyte
    cmd.operating_mode  = 0b010; // Rate generator (2)
    cmd.bcd_binary_mode = 0;     // Binary mode

    // Set the mode/command, then write the reload value into the PIT (low byte
    // first).
    outb(COMMAND_REGISTER, cmd.as_byte);
    outb(CHANNEL_0_REGISTER, (uint8_t)_reload_value);
    outb(CHANNEL_0_REGISTER, (uint8_t)(_reload_value >> 8));

    _log.debug("Set PIT frequency to {}Hz (requested frequency: {}Hz, reload value: {})\n", 
        BASE_FREQUENCY_HZ / _reload_value, in_frequency, _reload_value);
}
