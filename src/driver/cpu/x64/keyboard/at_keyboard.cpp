#include "at_keyboard.hpp"

at_keyboard::at_keyboard(logger& in_log, ps2_controller& in_ps2) :
    _log(in_log), _ps2(in_ps2) {
    _ps2.temp_dump();
    _log.debug("Constructed AT keyboard\n");
}

void at_keyboard::reset() {
    _log.debug("Reset AT keyboard\n");
}