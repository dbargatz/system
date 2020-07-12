#include "pit.hpp"
#include "../std/panic.h"

PIT::PIT(logger& in_log) : _log(in_log), _uptime_ms(0.0),
    _frequency_hz(MIN_FREQUENCY_HZ), _reload_value(0) { }

float64_t PIT::get_frequency() {
    return _frequency_hz;
}

void PIT::set_frequency(float64_t in_frequency_hz) {
    // If the frequency is too low or too high, panic.
    if(in_frequency_hz > BASE_FREQUENCY_HZ) {
        PANIC("frequency too high");
    } else if(in_frequency_hz < MIN_FREQUENCY_HZ) {
        PANIC("frequency too low");
    }

    // Calculate the new reload value.
    auto new_value = (BASE_FREQUENCY_HZ / in_frequency_hz);
    _reload_value = (uint16_t)new_value;
    auto delta = new_value - _reload_value;
    if(delta > 0.5) {
        _reload_value++;
    }

    // Calculate the actual frequency.
    _frequency_hz = (BASE_FREQUENCY_HZ / (float64_t)_reload_value);

    // Install the new reload value in the actual hardware.
    union _command_reg cmd;
    cmd.select_channel  = 0b00;  // Channel 0
    cmd.access_mode     = 0b11;  // Read/write lobyte,hibyte
    cmd.operating_mode  = 0b010; // Rate generator (2)
    cmd.bcd_binary_mode = 0;     // Binary mode

    // Set the mode/command, then write the reload value into the PIT (low byte
    // first).
    COMMAND_REGISTER.outb(cmd.as_byte);
    CHANNEL_0_REGISTER.outb((uint8_t)_reload_value);
    CHANNEL_0_REGISTER.outb((uint8_t)(_reload_value >> 8));

    _log.debug("Set PIT frequency to {}Hz (requested frequency: {}Hz, reload value: {})", 
        _frequency_hz, in_frequency_hz, _reload_value);
}

void PIT::interrupt_handler(interrupt_frame& in_frame) {
    _uptime_ms += 1000.0 / _frequency_hz;
    if((uint64_t)_uptime_ms % 1000 == 0 && _uptime_ms > 1.0) {
        _log.debug("Uptime: {}ms", _uptime_ms);
    }
}
