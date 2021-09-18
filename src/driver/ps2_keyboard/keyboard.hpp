#ifndef _KEYBOARD_KEYBOARD_HPP
#define _KEYBOARD_KEYBOARD_HPP

#include "../interrupts/frame.hpp"

class keyboard {
public:
    virtual ~keyboard() {};
    virtual void interrupt_handler(interrupt_frame& in_frame) = 0;
    virtual void reset() = 0;
};

#endif // _KEYBOARD_KEYBOARD_HPP