#ifndef _KEYBOARD_KEYSTATE_HPP
#define _KEYBOARD_KEYSTATE_HPP

#include <cstdint>
#include <string>
#include "scancode_set.hpp"

class keystate {
public:
    keystate();
    std::string set_key(keycode& in_code);

private:
    bool _alt         = false;
    bool _caps_lock   = false;
    bool _ctrl        = false;
    bool _num_lock    = false;
    bool _scroll_lock = false;
    bool _shift       = false;

    std::uint64_t _state[256][4];
};

#endif // _KEYBOARD_KEYSTATE_HPP
