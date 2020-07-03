#include "ps2_keyboard.hpp"

ps2_keyboard::ps2_keyboard(logger& in_log, ps2_controller& in_ps2, ps2_port in_port) :
    _log(in_log), _port(in_port), _ps2(in_ps2) {}

void ps2_keyboard::interrupt_handler(InterruptManager& in_mgr, interrupt_frame& in_frame) {
    auto scancode = _ps2.read(false);
    in_mgr.handler_complete(InterruptType::KEYBOARD);
    _log.debug("Keyboard: scancode {#02X}", scancode);
}

void ps2_keyboard::reset() {
    // Reset this keyboard the correct port.
    _ps2.enable(_port);
    // TODO: this is bad. implement as command queue/state machine to interact with interrupt handler.
    _ps2.write(_port, 0xFF);
    _ps2.write(_port, 0xEE);
    _log.debug("Reset PS/2 keyboard");
}
