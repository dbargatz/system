#ifndef _KEYBOARD_SCANCODE_SET_HPP
#define _KEYBOARD_SCANCODE_SET_HPP

#include <cstdint.hpp>
#include <format.hpp>
#include <string.hpp>

using scancode = std::uint64_t;

class keycode {
public:
    const std::uint8_t row;
    const std::uint8_t column;
    const bool pressed;

    constexpr keycode(std::uint8_t in_row, std::uint8_t in_column, bool in_pressed) :
        row(in_row), column(in_column), pressed(in_pressed) {};

    inline bool operator==(const keycode& in_other) {
        return (row == in_other.row && column == in_other.column);
    }

    std::string format() {
        return std::format(u8"Keycode {}.{}.{}", row, column,
            pressed ? u8"press" : u8"release");
    }
};

constexpr static const keycode INVALID_KEYCODE{255, 0, false};
constexpr static const keycode PARTIAL_KEYCODE{255, 1, false};

struct scancode_to_keycode {
    std::string scancode_name;
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
