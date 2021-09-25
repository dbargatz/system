#include "ps2_keyboard.hpp"
#include <cassert>
#include "scancode_set_2.hpp"
#include <cstdlib>

ps2_keyboard::ps2_keyboard(logging::logger& in_log, ps2_controller& in_ps2, scancode_set& in_scancode_set) :
    _log(in_log), _ps2(in_ps2), _cur_scancode_set(in_scancode_set) {
    _cur_state = _state::IDLE;
}

// TODO: This implementation is gross. Convert to an instance of a 
//       C++ state machine; see http://www.ludvikjerabek.com/2016/02/08/c11-state-machines/
//       for examples.
void ps2_keyboard::interrupt_handler(interrupt_frame& in_frame) {
    // Get the current data byte from the keyboard.
    auto data = _ps2.read(false);
    struct _command dummy = { _command_byte::ECHO, 0, {}};
    struct _command& cmd = (_commands.size() > 0 ? _commands.peek() : dummy);

    // If it's a resend request (which most commands must handle), verify it's
    // appropriate for the current state, then resend the command and exit.
    if((_response_byte)data == _response_byte::RESEND) {
        assert(_cur_state != _state::IDLE);
        assert(_cur_state != _state::WAITING_FOR_MORE_SCANCODE);
        assert(_cur_state != _state::WAITING_FOR_ID);
        assert(_commands.size() > 0);
        assert(_resend_count <= MAX_RESENDS);

        // Move to idle without removing last command; it needs to
        // be resent.
        _log.debug(u8"Keyboard requested re-send of command.");
        _resend_count++;
        _send_command();
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
                _log.error(u8"Invalid keycode conversion!");
                _cur_scancode = 0;
            } else {
                auto unicode = _keystate.set_key(keycode);
                if(unicode.length() > 0) { 
                    _log.debug(u8"{}", unicode);
                }
                _cur_scancode = 0;
            }
            break;
        }
        case _state::WAITING_FOR_ACK:
            assert((_response_byte)data == _response_byte::ACK);

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
                _log.debug(u8"Command complete.");
                _complete_command();
            }
            break;
        case _state::WAITING_FOR_SELF_TEST:
            assert((_response_byte)data == _response_byte::SELF_TEST_PASSED);
            _log.debug(u8"Keyboard passed self-test.");
            _complete_command();
            break;
        case _state::WAITING_FOR_ECHO:
            assert((_response_byte)data == _response_byte::ECHO);
            _log.debug(u8"Echo complete.");
            _complete_command();
            break;
        case _state::WAITING_FOR_SCANCODE_SET:
            assert(data == 0x02);
            _log.debug(u8"Keyboard using scancode set 2.");
            _complete_command();
            break;
        case _state::WAITING_FOR_ID:
            _log.debug(u8"Keyboard is type {02X}", data);
            _complete_command();
            break;
        case _state::WAITING_FOR_PREV_BYTE:
            _log.debug(u8"Last keyboard byte was {02X}", data);
            _complete_command();
            break;
        default:
            _log.error(u8"Last keyboard byte was {02X}", data);
            std::abort();
            break;
    }

    // If there are commands to be sent and we're not in a waiting state, send
    // the next command.
    _send_command();
}

void ps2_keyboard::reset() {
    // Determine which PS/2 port the keyboard is on.
    auto port1_type = _ps2.get_type(ps2_port::PORT1);
    _log.debug(u8"PS/2 port 1: {}", _ps2.get_type_str(port1_type));
    auto port2_type = _ps2.get_type(ps2_port::PORT2);
    _log.debug(u8"PS/2 port 2: {}", _ps2.get_type_str(port2_type));

    if(ps2_device_type::KEYBOARD_STANDARD == port1_type) {
        _port = ps2_port::PORT1;
    } else if(ps2_device_type::KEYBOARD_STANDARD == port2_type) {
        _port = ps2_port::PORT2;
    }

    // Reset this keyboard the correct port.
    _ps2.enable(_port);
    
    _commands.enqueue({ _command_byte::RESET, 0, {} });
    _commands.enqueue({ _command_byte::ECHO,  0, {} });

    _send_command();
    _log.debug(u8"Reset PS/2 keyboard");
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
        _ps2.write(_port, (std::uint8_t)cmd.cmd);
        for(auto i = 0; i < cmd.num_args; i++) {
            _ps2.write(_port, cmd.args[i]);
        }
    }
}
