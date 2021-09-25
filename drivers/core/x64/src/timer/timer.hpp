/**
 * @file
 * @brief Contains interface that all timer hardware must implement.
 */
#ifndef _CORE_X64_TIMER_TIMER_HPP
#define _CORE_X64_TIMER_TIMER_HPP

#include <cstdint>
#include "../interrupts/frame.hpp"

namespace core::x64::timer {

/**
 * @brief Abstract representation of timer hardware.
 */
class timer {
public:
    virtual ~timer() {};

    virtual double get_frequency() = 0;
    virtual void set_frequency(double in_frequency_hz) = 0;
    virtual void interrupt_handler(core::x64::interrupts::stack_frame& in_frame) = 0;
};

}; // namespace core::x64::timer

#endif // _CORE_X64_TIMER_TIMER_HPP
