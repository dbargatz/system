#include "pit.hpp"
#include "../std/panic.h"

core::x64::timer::pit::pit(logging::logger& in_log) : _log(in_log), _uptime_ms(0.0),
    _frequency_hz(MIN_FREQUENCY_HZ), _reload_value(0) { }

double core::x64::timer::pit::get_frequency() {
    return _frequency_hz;
}

void core::x64::timer::pit::set_frequency(double in_frequency_hz) {
    // If the frequency is too low or too high, panic.
    if(in_frequency_hz > BASE_FREQUENCY_HZ) {
        PANIC(u8"frequency too high");
    } else if(in_frequency_hz < MIN_FREQUENCY_HZ) {
        PANIC(u8"frequency too low");
    }

    // Calculate the new reload value.
    auto new_value = (BASE_FREQUENCY_HZ / in_frequency_hz);
    _reload_value = (std::uint16_t)new_value;
    auto delta = new_value - _reload_value;
    if(delta > 0.5) {
        _reload_value++;
    }

    // Calculate the actual frequency.
    _frequency_hz = (BASE_FREQUENCY_HZ / (double)_reload_value);

    // Install the new reload value in the actual hardware.
    union _command_reg cmd;
    cmd.select_channel  = 0b00;  // Channel 0
    cmd.access_mode     = 0b11;  // Read/write lobyte,hibyte
    cmd.operating_mode  = 0b010; // Rate generator (2)
    cmd.bcd_binary_mode = 0;     // Binary mode

    // Set the mode/command, then write the reload value into the pit (low byte
    // first).
    COMMAND_REGISTER.outb(cmd.as_byte);
    CHANNEL_0_REGISTER.outb((std::uint8_t)_reload_value);
    CHANNEL_0_REGISTER.outb((std::uint8_t)(_reload_value >> 8));

    _log.debug(u8"Set PIT frequency to {}Hz (requested frequency: {}Hz, reload value: {:#02X})", 
        _frequency_hz, in_frequency_hz, _reload_value);
}

void core::x64::timer::pit::interrupt_handler(core::x64::interrupts::stack_frame& in_frame) {
    _uptime_ms += 1000.0 / _frequency_hz;
    if((std::uint64_t)_uptime_ms % 10000 == 0 && _uptime_ms > 1.0) {
        _log.debug(u8"Uptime: {}ms", _uptime_ms);
    }
}
