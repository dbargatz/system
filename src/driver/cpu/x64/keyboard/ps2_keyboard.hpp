#ifndef _KEYBOARD_AT_KEYBOARD_HPP
#define _KEYBOARD_AT_KEYBOARD_HPP

#include "keyboard.hpp"
#include "ps2_controller.hpp"
#include "../interrupts/frame.hpp"
#include "../interrupts/interrupt_manager.hpp"
#include "../std/logger.hpp"

class at_keyboard : public keyboard {
public:
    at_keyboard(logger& in_log, ps2_controller& in_ps2, ps2_port in_port);
    void interrupt_handler(InterruptManager& in_mgr, interrupt_frame& in_frame) override;
    void reset() override;

private:
    logger& _log;
    ps2_port _port;
    ps2_controller& _ps2;
};

#endif // _KEYBOARD_AT_KEYBOARD_HPP