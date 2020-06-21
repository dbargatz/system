#ifndef _KEYBOARD_KEYBOARD_HPP
#define _KEYBOARD_KEYBOARD_HPP

#include "../interrupts/frame.hpp"
#include "../interrupts/interrupt_manager.hpp"

class keyboard {
public:
    virtual void interrupt_handler(InterruptManager& in_mgr, interrupt_frame& in_frame) = 0;
    virtual void reset() = 0;
};

#endif // _KEYBOARD_KEYBOARD_HPP