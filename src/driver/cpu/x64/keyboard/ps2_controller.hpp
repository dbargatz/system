#ifndef _KEYBOARD_PS2_CONTROLLER_HPP
#define _KEYBOARD_PS2_CONTROLLER_HPP

#include "../ports/io_port.hpp"
#include "../std/logger.hpp"
#include "../std/stdint.h"

enum class ps2_port : uint8_t {
    INVALID = 0,
    PORT1,
    PORT2,
    CONTROLLER
};

enum class ps2_device_type : uint8_t {
    INVALID = 0,
    MOUSE_STANDARD,
    MOUSE_SCROLL,
    MOUSE_5_BUTTON,
    KEYBOARD_TRANSLATED,
    KEYBOARD_STANDARD
};

class ps2_controller {
public:
    ps2_controller(logger& in_log);
    void disable(ps2_port in_port);
    ps2_device_type get_type(ps2_port in_port);
    const char * get_type_str(ps2_device_type in_type);
    const char * get_type_str(ps2_port in_port);
    uint8_t read();
    uint8_t write(ps2_port in_port, uint8_t in_data, uint8_t in_resend=0, uint8_t in_ack=0, bool in_response=false);

private:
    constexpr static const io_port PS2_DATA_REGISTER = io_port(0x0060);
    constexpr static const io_port PS2_STATUS_CMD_REGISTER = io_port(0x0064);

    bool _port_1_ok;
    bool _port_2_ok;
    logger& _log;

    uint8_t _read_config();
    void _write_config(uint8_t in_data);

    uint8_t _write_cmd(uint8_t in_data, bool in_response=false);
    void _write_data(uint8_t in_data);
};

#endif // _KEYBOARD_PS2_CONTROLLER_HPP