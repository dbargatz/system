#include "at_keyboard.hpp"

at_keyboard::at_keyboard(logger& in_log, ps2_controller& in_ps2, ps2_port in_port) :
    _log(in_log), _port(in_port), _ps2(in_ps2) {}

void at_keyboard::reset() {
    _log.debug("Reset AT keyboard\n");

    // Reset this keyboard the correct port.
    auto response = _ps2.write(_port, 0xFF, 0xFE, 0xFA, true);
    if(response != 0xAA) {
        _log.error("Keyboard on port {} failed reset (response {#02X})\n", 
                    (uint8_t)_port, response);
        _ps2.disable(_port);
    }
}