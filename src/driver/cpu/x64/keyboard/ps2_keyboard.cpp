#include "ps2_keyboard.hpp"
#include "scancode_set_2.hpp"
#include "../std/assert.h"
#include "../std/panic.h"

scancode_set_2 ps2_keyboard::set2;

ps2_keyboard::ps2_keyboard(logger& in_log, ps2_controller& in_ps2, ps2_port in_port) :
    _log(in_log), _port(in_port), _ps2(in_ps2), _cur_scancode_set(*(&set2)) {
    _cur_state = _state::IDLE;
}

// TODO: This implementation is gross. Convert to an instance of a 
//       C++ state machine; see http://www.ludvikjerabek.com/2016/02/08/c11-state-machines/
//       for examples.
void ps2_keyboard::interrupt_handler(InterruptManager& in_mgr, interrupt_frame& in_frame) {
    // Get the current data byte from the keyboard.
    uint8_t data = _ps2.read(false);
    struct _command dummy = { _command_byte::ECHO, 0, {}};
    struct _command& cmd = (_commands.size() > 0 ? _commands.peek() : dummy);

    // If it's a resend request (which most commands must handle), verify it's
    // appropriate for the current state, then resend the command and exit.
    if((_response_byte)data == _response_byte::RESEND) {
        ASSERT(_cur_state != _state::IDLE, "Received resend request in IDLE");
        ASSERT(_cur_state != _state::WAITING_FOR_MORE_SCANCODE, "Received resend request while in middle of scancode");
        ASSERT(_cur_state != _state::WAITING_FOR_ID, "Received resend request in WAITING_FOR_ID");
        ASSERT(_commands.size() > 0, "Received resend with no commands in-flight");
        ASSERT(_resend_count <= MAX_RESENDS, "exceeded max resends");

        // Move to idle without removing last command; it needs to
        // be resent.
        _log.debug("Keyboard requested re-send of command.");
        _resend_count++;
        _send_command();
        in_mgr.handler_complete(InterruptType::KEYBOARD);
        return;
    }

    // Otherwise, enter the state machine for handling the byte.
    switch(_cur_state) {
        case _state::IDLE:
        case _state::WAITING_FOR_MORE_SCANCODE:
        {
            _cur_scancode = (_cur_scancode << 8) + data;
            auto keycode = _cur_scancode_set.convert(_cur_scancode);
            if(keycode == PARTIAL_KEYCODE) {
                _cur_state = _state::WAITING_FOR_MORE_SCANCODE;
            } else if(keycode == INVALID_KEYCODE) {
                _log.error("Invalid keycode conversion!");
                _cur_scancode = 0;
            } else {
                auto unicode = _keystate.set_key(keycode);
                if(unicode.length() > 0) { 
                    _log.debug("{}", unicode);
                }
                _cur_scancode = 0;
            }
            break;
        }
        case _state::WAITING_FOR_ACK:
            ASSERT((_response_byte)data == _response_byte::ACK, "Received non-ACK");

            // Determine which state to go into next based on the completed
            // command.
            if(cmd.cmd == _command_byte::SCAN_CODE_SET &&
               (_scan_code_set)cmd.args[0] == _scan_code_set::GET_CURRENT) {
                _cur_state = _state::WAITING_FOR_SCANCODE_SET;
            } else if(cmd.cmd == _command_byte::IDENTIFY) {
                _cur_state = _state::WAITING_FOR_ID;
            } else if(cmd.cmd == _command_byte::RESET) {
                _cur_state = _state::WAITING_FOR_SELF_TEST;
            } else {
                _log.debug("Command complete.");
                _complete_command();
            }
            break;
        case _state::WAITING_FOR_SELF_TEST:
            ASSERT((_response_byte)data == _response_byte::SELF_TEST_PASSED, "Keyboard failed self-test");
            _log.debug("Keyboard passed self-test.");
            _complete_command();
            break;
        case _state::WAITING_FOR_ECHO:
            ASSERT((_response_byte)data == _response_byte::ECHO, "Received non-echo");
            _log.debug("Echo complete.");
            _complete_command();
            break;
        case _state::WAITING_FOR_SCANCODE_SET:
            ASSERT(data == 0x02, "Unknown/unsupported scancode set");
            _log.debug("Keyboard using scancode set 2.");
            _complete_command();
            break;
        case _state::WAITING_FOR_ID:
            _log.debug("Keyboard is type {02X}", data);
            _complete_command();
            break;
        case _state::WAITING_FOR_PREV_BYTE:
            _log.debug("Last keyboard byte was {02X}", data);
            _complete_command();
            break;
        default:
            PANIC("PS/2 keyboard in unknown state");
            break;
    }

    // If there are commands to be sent and we're not in a waiting state, send
    // the next command.
    _send_command();
    in_mgr.handler_complete(InterruptType::KEYBOARD);
}

void ps2_keyboard::reset() {
    // Reset this keyboard the correct port.
    _ps2.enable(_port);
    
    _commands.enqueue({ _command_byte::RESET, 0, {} });
    _commands.enqueue({ _command_byte::ECHO,  0, {} });

    _send_command();
    _log.debug("Reset PS/2 keyboard");
}

void ps2_keyboard::_complete_command() {
    _commands.dequeue();
    _resend_count = 0;
    _cur_state = _state::IDLE;
}

void ps2_keyboard::_send_command() {
    if(_cur_state == _state::IDLE && _commands.size() > 0) {
        struct _command& cmd = _commands.peek();
        switch(cmd.cmd) {
            case _command_byte::ECHO:
                _cur_state = _state::WAITING_FOR_ECHO;
                break;
            case _command_byte::RESEND:
                _cur_state = _state::WAITING_FOR_PREV_BYTE;
                break;
            default:
                _cur_state = _state::WAITING_FOR_ACK;
                break;
        };

        // Send the actual command, followed by any args it specifies.
        _ps2.write(_port, (uint8_t)cmd.cmd);
        for(auto i = 0; i < cmd.num_args; i++) {
            _ps2.write(_port, cmd.args[i]);
        }
    }
}
