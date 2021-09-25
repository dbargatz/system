#ifndef _KEYBOARD_KEYBOARD_HPP
#define _KEYBOARD_KEYBOARD_HPP

#include "../../core/x64/src/interrupts/frame.hpp"

class keyboard {
public:
    virtual ~keyboard() {};
    virtual void interrupt_handler(core::x64::interrupts::stack_frame& in_frame) = 0;
    virtual void reset() = 0;
};

#endif // _KEYBOARD_KEYBOARD_HPP