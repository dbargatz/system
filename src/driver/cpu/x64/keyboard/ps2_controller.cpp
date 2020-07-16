#include "ps2_controller.hpp"
#include "../std/panic.h"

ps2_device_type ps2_controller::get_type(ps2_port in_port) {
    ps2_device_type type = ps2_device_type::INVALID;

    // Disable scanning on the device.
    write(in_port, 0xF5, 0xFE, 0xFA);

    // Send the identify command to the device.
    auto response = write(in_port, 0xF2, 0xFE, 0xFA, true);

    switch(response) {
        case 0x00:
            type = ps2_device_type::MOUSE_STANDARD;
            break;
        case 0x03:
            type = ps2_device_type::MOUSE_SCROLL;
            break;
        case 0x04:
            type = ps2_device_type::MOUSE_5_BUTTON;
            break;
        case 0xAB:
            // Start of a 2-byte response.
            break;
        default:
            _log.warn("Invalid PS/2 device type {#02X} receive", response);
            type = ps2_device_type::INVALID;
            break;
    }

    // Handle multi-byte types.
    if(0xAB == response) {
        response = read(true);
        switch(response) {
            case 0x41: // Intentional fall-through
            case 0xC1:
                type = ps2_device_type::KEYBOARD_TRANSLATED;
                break;
            case 0x83:
                type = ps2_device_type::KEYBOARD_STANDARD;
                break;
            default:
                _log.warn("Invalid PS/2 device type (0xAB, {#02X}) received", response);
                type = ps2_device_type::INVALID;
                break;
        }
    }

    // Re-enable scanning on the device.
    write(in_port, 0xF4, 0xFE, 0xFA);
    return type;
}

const char * ps2_controller::get_type_str(ps2_device_type in_type) {
    switch(in_type) {
        case ps2_device_type::INVALID:
            return "invalid";
        case ps2_device_type::MOUSE_STANDARD:
            return "standard mouse";
        case ps2_device_type::MOUSE_SCROLL:
            return "mouse with scroll wheel";
        case ps2_device_type::MOUSE_5_BUTTON:
            return "5-button mouse";
        case ps2_device_type::KEYBOARD_TRANSLATED:
            return "keyboard (translation enabled)";
        case ps2_device_type::KEYBOARD_STANDARD:
            return "standard keyboard";
        default:
            return "unknown type";
    }
}

const char * ps2_controller::get_type_str(ps2_port in_port) {
    ps2_device_type type = get_type(in_port);
    return get_type_str(type);
}

void ps2_controller::disable(ps2_port in_port) {
    switch(in_port) {
        case ps2_port::PORT1:
            _port_1_ok = false;
            _write_cmd(0xAD); // Disable port 1
            _log.debug("Disabled PS/2 port 1.");
            return;
        case ps2_port::PORT2:
            _port_2_ok = false;
            _write_cmd(0xA7); // Disable port 2
            _log.debug("Disabled PS/2 port 2.");
            return;
        case ps2_port::CONTROLLER:
            disable(ps2_port::PORT1);
            disable(ps2_port::PORT2);
            _log.debug("Disabled both PS/2 ports.");
            return;
        default:
            _log.warn("Invalid PS/2 device; cannot disable.");
            return;
    }
}

void ps2_controller::enable(ps2_port in_port) {
    uint8_t config;
    switch(in_port) {
        case ps2_port::PORT1:
            if(!_port_1_ok) {
                _log.warn("Cannot enable PS/2 port 1; in bad state.");
                return;
            }
            _write_cmd(0xAE);                          // Enable port 1
            config = _read_config();
            _write_config(config | 0b00000001);        // Enable port 1 interrupts
            _log.debug("Enabled PS/2 port 1.");
            _port_1_ok = true;
            return;
        case ps2_port::PORT2:
            if(!_port_2_ok) {
                _log.warn("Cannot enable PS/2 port 2; in bad state.");
                return;
            }
            _write_cmd(0xA8);                          // Enable port 2
            config = _read_config();
            _write_config(config | 0b00000010);        // Enable port 2 interrupts
            _log.debug("Enabled PS/2 port 2.");
            _port_1_ok = true;
            return;
        case ps2_port::CONTROLLER:
            enable(ps2_port::PORT1);
            enable(ps2_port::PORT2);
            _log.debug("Enabled both PS/2 ports.");
            return;
        default:
            _log.warn("Invalid PS/2 device; cannot enable.");
            return;
    }
}

uint8_t ps2_controller::read(bool in_poll) {
    // Spin-wait until there's data available according to bit 0 of the 
    // controller's status register.
    while(in_poll && !(PS2_STATUS_CMD_REGISTER.inb() & 0x01));
    auto response = PS2_DATA_REGISTER.inb();
    return response;
}

void ps2_controller::reset() {
    // Disable the first and second PS/2 ports. If this is a single-port
    // controller, the disable command for port 2 will be ignored.
    _write_cmd(0xAD); // Disable port 1
    _write_cmd(0xA7); // Disable port 2

    // Flush the controller's output buffer to clear out any old data.
    while(PS2_STATUS_CMD_REGISTER.inb() & 0x01) {
        _log.debug("Old PS/2 data in buffer: {#02X", PS2_DATA_REGISTER.inb());
    }

    // Configure the controller to disable interrupts for ports 1 and 2, and
    // disable translation from scan code set 1 to scan code set 2.
    auto config = _read_config();
    _write_config(config & 0b10111100); // Clear bits 0, 1, 6

    // Run the PS/2 controller's self-test. Indicate both ports are unusable if
    // it fails.
    auto response = _write_cmd(0xAA, true);
    if(response != 0x55) {
        _log.error("PS/2 controller failed self-test (response: {#02X})");
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

    // Test port 1.
    _port_1_ok = true;
    response = _write_cmd(0xAB, true);
    if(response != 0x00) {
        _log.error("PS/2 port 1 failed test (response {#02X})", response);
        _port_1_ok = false;
    }

    // Test port 2 (if present).
    if(_port_2_ok) {
        response = _write_cmd(0xA9, true);
        if(response != 0x00) {
            _log.error("PS/2 port 2 failed test (response {#02X})", response);
            _port_2_ok = false;
        }
    }

    _log.debug("PS/2 controller: reset, {}-channel", _port_2_ok ? "dual" : "single");
}

uint8_t ps2_controller::write(ps2_port in_port, uint8_t in_data, uint8_t in_resend, uint8_t in_ack, bool in_response) {
    uint8_t response = 0;

    // TODO: Verify port is OK for writing (_port_1_ok/_port_2_ok/in_port not invalid)

    // Select the second port for writing, if necessary.
    if(in_port == ps2_port::PORT2) {
        _write_cmd(0xD4);
    }

    while(true) {
        // Write out the given data.
        _write_data(in_data);

        if(0 == in_resend && 0 == in_ack && !in_response) {
            break;
        }

        // Grab the response. If it's a resend request, loop; if it's an ack,
        // grab the next byte and head out; otherwise, it's an error.
        response = read(true);
        if(0x00 != in_resend && response == in_resend && in_response) {
            continue;
        } else if(0x00 != in_ack && response == in_ack && in_response) {
            response = read(true);
            break;
        } else {
            break;
        }
    }

    return response;
}

uint8_t ps2_controller::_read_config() {
    // Write the "read byte 0" command to the controller, wait, and return the
    // config byte.
    return _write_cmd(0x20, true);
}

uint8_t ps2_controller::_write_cmd(uint8_t in_data, bool in_response) {
    // Spin-wait while the controller's command input buffer (data going to the
    // controller) is full according to the controller's status register bit 1.
    while(PS2_STATUS_CMD_REGISTER.inb() & 0x02);
    PS2_STATUS_CMD_REGISTER.outb(in_data);
    return (in_response ? read(true) : 0);
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

ps2_controller::ps2_controller(logger& in_log) : _log(in_log) { }
