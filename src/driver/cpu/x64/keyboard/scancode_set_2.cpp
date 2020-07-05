#include "scancode_set_2.hpp"

keycode scancode_set_2::convert(scancode in_code) {
    // TODO: implement
    switch(in_code) {
        case 0xE0:
        case 0xE1:
        case 0xF0:
        case 0xE0F0:
        case 0xE114:
        case 0xE012E0:
        case 0xE0F07C:
        case 0xE11477:
        case 0xE0F07CE0:
        case 0xE11477E1:
        case 0xE0F07cE0F0:
        case 0xE11477E1F0:
        case 0xE11477E1F014:
        case 0xE11477E1F014F0:
            // These are all potentially-valid prefixes for press/release
            // scancodes in this set.
            return PARTIAL_KEYCODE;
        default:
            for(auto& entry : mappings) {
                if(entry.press == in_code) {
                    return keycode(entry.key_press.row, entry.key_press.column, true);
                } else if(entry.release == in_code) {
                    return keycode(entry.key_release.row, entry.key_release.column, false);
                }
            }
            return INVALID_KEYCODE;
    }
}
