#ifndef _KEYBOARD_SCANCODE_SET_HPP
#define _KEYBOARD_SCANCODE_SET_HPP

#include "../std/assert.h"
#include "../std/stdint.h"
#include "../std/text.hpp"

typedef uint64_t scancode;

class keycode {
public:
    const uint8_t row;
    const uint8_t column;
    const bool pressed;

    constexpr keycode(uint8_t in_row, uint8_t in_column, bool in_pressed) :
        row(in_row), column(in_column), pressed(in_pressed) {};

    inline bool operator==(const keycode& in_other) {
        return (row == in_other.row && column == in_other.column);
    }

    text format() {
        return text("Keycode {}.{}.{}", row, column,
            pressed ? "press" : "release");
    }
};

constexpr static const keycode INVALID_KEYCODE{255, 0, false};
constexpr static const keycode PARTIAL_KEYCODE{255, 1, false};

struct scancode_to_keycode {
    text scancode_name;
    scancode press;
    keycode key_press;
    scancode release;
    keycode key_release;
};

class scancode_set {
public:
    virtual ~scancode_set() {};
    virtual keycode convert(scancode in_code)    = 0;
};

#endif // _KEYBOARD_SCANCODE_SET_HPP
