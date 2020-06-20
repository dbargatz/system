#include "ps2_controller.hpp"
#include "../std/panic.h"

uint8_t ps2_controller::_read_config() {
    // Write the "read byte 0" command to the controller, wait, and return the
    // config byte.
    return _write_cmd(0x20, true);
}

uint8_t ps2_controller::_read_data() {
    // Spin-wait until there's data available according to bit 0 of the 
    // controller's status register.
    while(!(PS2_STATUS_CMD_REGISTER.inb() & 0x01));
    return PS2_DATA_REGISTER.inb();
}

uint8_t ps2_controller::_write_cmd(uint8_t in_data, bool in_response) {
    // Spin-wait while the controller's command input buffer (data going to the
    // controller) is full according to the controller's status register bit 1.
    while(PS2_STATUS_CMD_REGISTER.inb() & 0x02);
    PS2_STATUS_CMD_REGISTER.outb(in_data);

    return (in_response ? _read_data() : 0);
}

void ps2_controller::_write_config(uint8_t in_config) {
    // Write the "write byte 0" command to the controller.
    _write_cmd(0x60);

    // Write the config to the device.
    _write_data(in_config);
}

void ps2_controller::_write_data(uint8_t in_data) {
    // Spin-wait while the controller's data output buffer (data going to the
    // controller or a device) is full according to the controller's status
    // register bit 1.
    while(PS2_STATUS_CMD_REGISTER.inb() & 0x02);
    PS2_DATA_REGISTER.outb(in_data);
}

uint8_t ps2_controller::_write_data(uint8_t in_data, uint8_t in_resend, uint8_t in_ack) {
    uint8_t response;
    while(true) {
        // Write out the given data.
        _write_data(in_data);

        // Grab the response. If it's a resend request, loop; if it's an ack,
        // grab the next byte and head out; otherwise, it's an error.
        response = _read_data();
        if(0x00 != in_resend && response == in_resend) {
            continue;
        } else if(0x00 != in_ack && response == in_ack) {
            response = _read_data();
            break;
        } else {
            break;
        }
    }

    return response;
}

uint8_t ps2_controller::_write_port1(uint8_t in_data, uint8_t in_resend, uint8_t in_ack) {
    return _write_data(in_data, in_resend, in_ack);
}

uint8_t ps2_controller::_write_port2(uint8_t in_data, uint8_t in_resend, uint8_t in_ack) {
    _write_cmd(0xD4);
    return _write_data(in_data, in_resend, in_ack);
}

ps2_controller::ps2_controller(logger& in_log) : _log(in_log) {
    // Disable the first and second PS/2 ports. If this is a single-port
    // controller, the disable command for port 2 will be ignored.
    _write_cmd(0xAD); // Disable port 1
    _write_cmd(0xA7); // Disable port 2

    // Flush the controller's output buffer to clear out any old data.
    while(PS2_STATUS_CMD_REGISTER.inb() & 0x01) {
        _log.debug("Old PS/2 data in buffer: {#02X}\n", PS2_DATA_REGISTER.inb());
    }

    // Configure the controller to disable interrupts for ports 1 and 2, and
    // disable translation from scan code set 1 to scan code set 2.
    auto config = _read_config();
    _write_config(config & 0b10111100); // Clear bits 0, 1, 6

    // Run the PS/2 controller's self-test. Indicate both ports are unusable if
    // it fails.
    auto response = _write_cmd(0xAA, true);
    if(response != 0x55) {
        _log.error("PS/2 controller failed self-test (response: {#02X})\n");
        _port_1_ok = false;
        _port_2_ok = false;
        return;
    }

    // Determine if this is a single-channel (1 port) or dual-channel (2-port)
    // PS/2 controller. If bit 5 of the config byte is set, it means the second
    // port is disabled; at this point, bit 5 should be set because we disabled
    // both ports. If bit 5 is clear, the controller is telling us there isn't
    // a second PS/2 port, since if there was one, the second port would be
    // disabled and bit 5 would be clear.
    if(config & 0b00100000) {
        // Briefly re-enable port 2 to ensure bit 5 is cleared, indicating the
        // second port is supported by this controller.
        _write_cmd(0xA8);                     // Enable port 2
        config = _read_config();
        _port_2_ok = !(config & 0b00100000); // Bit 5 should be clear now
        if(_port_2_ok) {
            _write_cmd(0xA7);                 // Disable port 2 again
        }
    }
    _log.debug("PS/2 is {}-channel\n", _port_2_ok ? "dual" : "single");

    // Test port 1.
    _port_1_ok = true;
    response = _write_cmd(0xAB, true);
    if(response != 0x00) {
        _log.error("PS/2 port 1 failed test (response {#02X})\n", response);
        _port_1_ok = false;
    }

    // Test port 2 (if present).
    if(_port_2_ok) {
        response = _write_cmd(0xA9, true);
        if(response != 0x00) {
            _log.error("PS/2 port 2 failed test (response {#02X})\n", response);
            _port_2_ok = false;
        }
    }

    // Enable the first PS/2 port, turn on interrupts, and reset the device to
    // determine if a device is present.
    if(_port_1_ok) {
        _write_cmd(0xAE);                          // Enable port 1
        config = _read_config();
        _write_config(config | 0b00000001);        // Enable port 1 interrupts

        response = _write_port1(0xFF, 0xFE, 0xFA); // Reset the device on port 1
        if(response != 0xAA) {
            _log.error("PS/2 device 1 failed reset (response {#02X})\n", response);
            _port_1_ok = false;
        }
    }

    // Enable the second PS/2 port, turn on interrupts, and reset the device to
    // determine if a device is present.
    if(_port_2_ok) {
        _write_cmd(0xA8);                          // Enable port 2
        config = _read_config();
        _write_config(config | 0b00000010);        // Enable port 2 interrupts

        response = _write_port2(0xFF, 0xFE, 0xFA); // Reset the device on port 2
        if(response != 0xAA) {
            _log.error("PS/2 device 2 failed reset (response {#02X})\n", response);
            _port_2_ok = false;
        }
    }
    _log.debug("PS/2: so far, so good!\n");
}

uint8_t ps2_controller::read() {
    PANIC("read() not implemented yet\n");
    return 0;
}

void ps2_controller::write(uint8_t in_data) {
    PANIC("write() not implemented yet\n");
}

void ps2_controller::temp_dump() {
    _log.debug("PS/2 controller:\n");
    _log.debug("\tPort 1: {}abled\n", _port_1_ok ? "en" : "dis");
    _log.debug("\tPort 2: {}abled\n", _port_2_ok ? "en" : "dis");
}