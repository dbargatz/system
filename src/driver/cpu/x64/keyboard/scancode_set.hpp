#ifndef _KEYBOARD_SCANCODE_SET_HPP
#define _KEYBOARD_SCANCODE_SET_HPP

#include "../std/stdint.h"
#include "../std/text.hpp"

typedef uint64_t scancode;

class keycode {
public:
    const uint8_t row;
    const uint8_t column;

    constexpr keycode(uint8_t in_row, uint8_t in_column) : row(in_row), column(in_column) {};

    inline bool operator==(const keycode& in_other) {
        return (row == in_other.row && column == in_other.column);
    }

    text name() {
        return text("Row {}, Column {}", row, column);
    }
};

constexpr static const keycode INVALID_KEYCODE{0, 0};
constexpr static const keycode PARTIAL_KEYCODE{0, 1};

struct scancode_to_keycode {
    text scancode_name;
    scancode press;
    scancode release;
    keycode key;
};

class scancode_set {
public:
    virtual keycode convert(scancode in_code)    = 0;
};

#endif // _KEYBOARD_SCANCODE_SET_HPP
