/**
 * @file
 * @brief Contains interface that all timer hardware must implement.
 */
#ifndef _TIMER_ITIMER_HPP
#define _TIMER_ITIMER_HPP

#include "../std/stdint.h"

/**
 * @brief Abstract representation of timer hardware.
 */
class ITimer {
public:
    virtual void set_frequency(uint32_t in_frequency_in_hz) = 0;
};

#endif // _TIMER_ITIMER_HPP
