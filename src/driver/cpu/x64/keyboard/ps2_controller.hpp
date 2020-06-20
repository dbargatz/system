#ifndef _KEYBOARD_PS2_CONTROLLER_HPP
#define _KEYBOARD_PS2_CONTROLLER_HPP

#include "../ports/io_port.hpp"
#include "../std/logger.hpp"
#include "../std/stdint.h"

class ps2_controller {
public:
    ps2_controller(logger& in_log);
    uint8_t read();
    void write(uint8_t in_data);
    void temp_dump();

private:
    constexpr static const io_port PS2_DATA_REGISTER = io_port(0x0060);
    constexpr static const io_port PS2_STATUS_CMD_REGISTER = io_port(0x0064);

    bool _port_1_ok;
    bool _port_2_ok;
    logger& _log;

    uint8_t _read_config();
    uint8_t _read_data();

    uint8_t _write_cmd(uint8_t in_data, bool in_response=false);
    void _write_data(uint8_t in_data);
    uint8_t _write_data(uint8_t in_data, uint8_t in_resend, uint8_t in_ack);
    void _write_config(uint8_t in_data);
    uint8_t _write_port1(uint8_t in_data, uint8_t in_resend, uint8_t in_ack);
    uint8_t _write_port2(uint8_t in_data, uint8_t in_resend, uint8_t in_ack);
};

#endif // _KEYBOARD_PS2_CONTROLLER_HPP