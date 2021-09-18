#ifndef _KEYBOARD_PS2_KEYBOARD_HPP
#define _KEYBOARD_PS2_KEYBOARD_HPP

#include <cstdint>
#include <queue>
#include <string>
#include "../../../../logging/logger.hpp"
#include "keyboard.hpp"
#include "keystate.hpp"
#include "ps2_controller.hpp"
#include "scancode_set.hpp"
#include "scancode_set_2.hpp"
#include "../interrupts/frame.hpp"

class ps2_keyboard : public keyboard {
public:
    ps2_keyboard(logging::logger& in_log, ps2_controller& in_ps2, scancode_set& in_scancode_set);
    void interrupt_handler(interrupt_frame& in_frame) override;
    void reset() override;

private:
    constexpr static const std::uint8_t MAX_RESENDS = 3;

    enum class _command_byte : std::uint8_t {
        SET_LED            = 0xED,
        ECHO               = 0xEE,
        SCAN_CODE_SET      = 0xF0,
        IDENTIFY           = 0xF2,
        SET_RATE_DELAY     = 0xF3,
        ENABLE_SCANNING    = 0xF4,
        DISABLE_SCANNING   = 0xF5,
        SET_DEFAULT_PARAMS = 0xF6,
        SET_ALL_TM_AR_ONLY = 0xF7,
        SET_ALL_M_R_ONLY   = 0xF8,
        SET_ALL_M_ONLY     = 0xF9,
        SET_ALL_TM_AR_M_R  = 0xFA,
        SET_ONE_TM_AR_ONLY = 0xFB,
        SET_ONE_M_R_ONLY   = 0xFC,
        SET_ONE_M_ONLY     = 0xFD,
        RESEND             = 0xFE,
        RESET              = 0xFF
    };

    enum class _response_byte : std::uint8_t {
        INTERNAL_ERROR1   = 0x00,
        SELF_TEST_PASSED  = 0xAA,
        ECHO              = 0xEE,
        ACK               = 0xFA,
        SELF_TEST_FAILED1 = 0xFC,
        SELF_TEST_FAILED2 = 0xFD,
        RESEND            = 0xFE,
        INTERNAL_ERROR2   = 0xFF
    };

    enum class _led_state : std::uint8_t {
        SCROLL_LOCK = 0x00,
        NUM_LOCK    = 0x01,
        CAPS_LOCK   = 0x02
    };

    enum class _scan_code_set : std::uint8_t {
        GET_CURRENT = 0x00,
        USE_SET_1   = 0x01,
        USE_SET_2   = 0x02,
        USE_SET_3   = 0x03
    };

    enum class _state : std::uint8_t {
        UNKNOWN = 0,
        IDLE,
        WAITING_FOR_ID,
        WAITING_FOR_ECHO,
        WAITING_FOR_PREV_BYTE,
        WAITING_FOR_SELF_TEST,
        WAITING_FOR_ACK,
        WAITING_FOR_SCANCODE_SET,
        WAITING_FOR_MORE_SCANCODE,
    };

    struct _command {
        _command_byte cmd;
        std::uint8_t num_args;
        std::uint8_t args[8];
    };

    struct _response {
        _response_byte response;
        std::uint8_t data[8];
    };

    void _complete_command();
    void _send_command();

    logging::logger& _log;
    ps2_port _port;
    ps2_controller& _ps2;

    _state _cur_state;
    std::uint8_t _resend_count;
    scancode _cur_scancode;
    scancode_set& _cur_scancode_set;
    std::queue<struct _command, 16> _commands;
    keystate _keystate;
};

#endif // _KEYBOARD_PS2_KEYBOARD_HPP
