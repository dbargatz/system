/**
 * @file
 * @brief Contains interface that all timer hardware must implement.
 */
#ifndef _TIMER_ITIMER_HPP
#define _TIMER_ITIMER_HPP

#include "../interrupts/frame.hpp"
#include "../std/stdint.h"

/**
 * @brief Abstract representation of timer hardware.
 */
class timer {
public:
    virtual float64_t get_frequency() = 0;
    virtual void set_frequency(float64_t in_frequency_hz) = 0;
    virtual void interrupt_handler(interrupt_frame& in_frame) = 0;
};

#endif // _TIMER_ITIMER_HPP
