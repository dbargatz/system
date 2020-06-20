#ifndef _KEYBOARD_AT_KEYBOARD_HPP
#define _KEYBOARD_AT_KEYBOARD_HPP

#include "keyboard.hpp"
#include "ps2_controller.hpp"
#include "../std/logger.hpp"

class at_keyboard : public keyboard {
public:
    at_keyboard(logger& in_log, ps2_controller& in_ps2);
    void reset() override;

private:
    logger& _log;
    ps2_controller& _ps2;
};

#endif // _KEYBOARD_AT_KEYBOARD_HPP