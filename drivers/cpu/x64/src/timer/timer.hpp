/**
 * @file
 * @brief Contains interface that all timer hardware must implement.
 */
#ifndef _TIMER_TIMER_HPP
#define _TIMER_TIMER_HPP

#include <cstdint>
#include "../interrupts/frame.hpp"

/**
 * @brief Abstract representation of timer hardware.
 */
class timer {
public:
    virtual ~timer() {};

    virtual double get_frequency() = 0;
    virtual void set_frequency(double in_frequency_hz) = 0;
    virtual void interrupt_handler(interrupt_frame& in_frame) = 0;
};

#endif // _TIMER_TIMER_HPP
