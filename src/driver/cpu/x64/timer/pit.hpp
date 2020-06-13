#ifndef _TIMER_PIT_H
#define _TIMER_PIT_H

#include "../interrupts/frame.hpp"
#include "../ports/io.h"
#include "../std/logger.hpp"

extern "C" void pit_handler(logger& in_log, interrupt_frame& in_frame);

class PIT {

public:
    ///< Read/write. When read, returns 16-bit count value. Written values are
    ///< "reload" values. Meaning of "count" and "reload" vary based on mode of
    ///< PIT, which is specified by the COMMAND_REGISTER.
    static const io_port_addr_t CHANNEL_0_REGISTER = 0x0040;

    ///< Write-only. Determines how a specified channel register will be read or
    ///< written to, and what mode the PIT channel operates in. See 
    ///< _command_reg for format.
    static const io_port_addr_t COMMAND_REGISTER        = 0x0043;

    ///< The base/maximum clock frequency of the PIT in Hz. Actual frequency is
    ///< 1.193181666... MHz (with repeating 6), so rounded up.
    static const uint32_t BASE_FREQUENCY_HZ = 1193182;

    ///< The slowest frequency the PIT can fire at, in Hz.
    static const uint32_t MIN_FREQUENCY_HZ = 18;

    union _command_reg {
        uint8_t as_byte;
        struct {
            ///< Whether PIT will operate in 16-bit binary (0) or BCD (1) modes. For
            ///< compatibility/fewer headaches, this should always be binary (0).
            uint8_t bcd_binary_mode : 1;
            ///< How selected channel will generate interrupts, and what values read
            ///< or written to the channel's register mean.
            uint8_t operating_mode  : 3;
            ///< How selected channel will be read/written (lobyte, hibyte, or 
            ///< both), or specifies the counter latch.
            uint8_t access_mode     : 2;
            ///< Which channel (0, 1, 2) is being configured, or the readback
            ///< command.
            uint8_t select_channel  : 2;
        };
    };

    logger& _log;
    uint16_t _reload_value;

    PIT(logger& in_log, uint32_t in_frequency);

    void set_frequency(uint32_t in_frequency);
};

#endif // _TIMER_PIT_H
