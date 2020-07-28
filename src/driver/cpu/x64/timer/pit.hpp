#ifndef _TIMER_PIT_HPP
#define _TIMER_PIT_HPP

#include <cstdint.hpp>
#include "../../../../logging/logger.hpp"
#include "timer.hpp"
#include "../interrupts/frame.hpp"
#include "../ports/io_port.hpp"
#include "../core.hpp"

class pit : public timer {
private:
    ///< Read/write. When read, returns 16-bit count value. Written values are
    ///< "reload" values. Meaning of "count" and "reload" vary based on mode of
    ///< PIT, which is specified by COMMAND_REGISTER.
    constexpr static const io_port CHANNEL_0_REGISTER = io_port(0x0040);

    ///< Write-only. Determines how a specified channel register will be read or
    ///< written to, and what mode the PIT channel operates in. See 
    ///< _command_reg for format.
    constexpr static const io_port COMMAND_REGISTER = io_port(0x0043);

    ///< The base/maximum clock frequency of the PIT in Hz. Actual frequency is
    ///< 1,193,181.666... Hz (with repeating 6).
    constexpr static const double BASE_FREQUENCY_HZ = 1193181.666666666666666;

    ///< The slowest frequency the PIT can fire at, in Hz.
    constexpr static const double MIN_FREQUENCY_HZ = BASE_FREQUENCY_HZ / UINT16_MAX;

    union _command_reg {
        std::uint8_t as_byte;
        struct {
            ///< Whether PIT will operate in 16-bit binary (0) or BCD (1) modes.
            ///< For max compatibility, this should always be binary (0).
            std::uint8_t bcd_binary_mode : 1;
            ///< How selected channel will generate interrupts, and what values
            ///< read or written to the channel's register mean.
            std::uint8_t operating_mode  : 3;
            ///< How selected channel will be read/written (lobyte, hibyte, or
            ///< both), or specifies the counter latch.
            std::uint8_t access_mode     : 2;
            ///< Which channel (0, 1, 2) is being configured, or the readback
            ///< command.
            std::uint8_t select_channel  : 2;
        };
    };

    logging::logger&   _log;
    double _uptime_ms;
    double _frequency_hz;
    std::uint16_t  _reload_value;

public:
    virtual ~pit() override {};
    pit(logging::logger& in_log);

    double get_frequency() override;
    void set_frequency(double in_frequency_hz) override;
    void interrupt_handler(interrupt_frame& in_frame) override;
};

#endif // _TIMER_PIT_HPP
