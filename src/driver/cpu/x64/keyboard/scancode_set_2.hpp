#ifndef _KEYBOARD_SCANCODE_SET_2_HPP
#define _KEYBOARD_SCANCODE_SET_2_HPP

#include "scancode_set.hpp"

class scancode_set_2 : public scancode_set {
public:
    struct scancode_to_keycode mappings[125] = {
        {u8"F9",                   0x01,   {  1,  10, true}, 0xF001, {  1,  10, false}},
        {u8"F5",                   0x03,   {  1,   6, true}, 0xF003, {  1,   6, false}},
        {u8"F3",                   0x04,   {  1,   4, true}, 0xF004, {  1,   4, false}},
        {u8"F1",                   0x05,   {  1,   2, true}, 0xF005, {  1,   2, false}},
        {u8"F2",                   0x06,   {  1,   3, true}, 0xF006, {  1,   3, false}},
        {u8"F12",                  0x07,   {  1,  13, true}, 0xF007, {  1,  13, false}},
        {u8"F10",                  0x09,   {  1,  11, true}, 0xF009, {  1,  11, false}},
        {u8"F8",                   0x0A,   {  1,   9, true}, 0xF00A, {  1,   9, false}},
        {u8"F6",                   0x0B,   {  1,   7, true}, 0xF00B, {  1,   7, false}},
        {u8"F4",                   0x0C,   {  1,   5, true}, 0xF00C, {  1,   5, false}},
        {u8"Tab",                  0x0D,   {  3,   1, true}, 0xF00D, {  3,   1, false}},
        {u8"Backtick",             0x0E,   {  2,   1, true}, 0xF00E, {  2,   1, false}},
        {u8"Left Alt",             0x11,   {  6,   3, true}, 0xF011, {  6,   3, false}},
        {u8"Left Shift",           0x12,   {  5,   1, true}, 0xF012, {  5,   1, false}},
        {u8"Left Control",         0x14,   {  6,   1, true}, 0xF014, {  6,   1, false}},
        {u8"Q",                    0x15,   {  3,   2, true}, 0xF015, {  3,   2, false}},
        {u8"1",                    0x16,   {  2,   2, true}, 0xF016, {  2,   2, false}},
        {u8"Z",                    0x1A,   {  5,   2, true}, 0xF01A, {  5,   2, false}},
        {u8"S",                    0x1B,   {  4,   3, true}, 0xF01B, {  4,   3, false}},
        {u8"A",                    0x1C,   {  4,   2, true}, 0xF01C, {  4,   2, false}},
        {u8"W",                    0x1D,   {  3,   3, true}, 0xF01D, {  3,   3, false}},
        {u8"2",                    0x1E,   {  2,   3, true}, 0xF01E, {  2,   3, false}},
        {u8"C",                    0x21,   {  5,   4, true}, 0xF021, {  5,   4, false}},
        {u8"X",                    0x22,   {  5,   3, true}, 0xF022, {  5,   3, false}},
        {u8"D",                    0x23,   {  4,   4, true}, 0xF023, {  4,   4, false}},
        {u8"E",                    0x24,   {  3,   4, true}, 0xF024, {  3,   4, false}},
        {u8"4",                    0x25,   {  2,   5, true}, 0xF025, {  2,   5, false}},
        {u8"3",                    0x26,   {  2,   4, true}, 0xF026, {  2,   4, false}},
        {u8"Space",                0x29,   {  6,   4, true}, 0xF029, {  6,   4, false}},
        {u8"V",                    0x2A,   {  5,   5, true}, 0xF02A, {  5,   5, false}},
        {u8"F",                    0x2B,   {  4,   5, true}, 0xF02B, {  4,   5, false}},
        {u8"T",                    0x2C,   {  3,   6, true}, 0xF02C, {  3,   6, false}},
        {u8"R",                    0x2D,   {  3,   5, true}, 0xF02D, {  3,   5, false}},
        {u8"5",                    0x2E,   {  2,   6, true}, 0xF02E, {  2,   6, false}},
        {u8"N",                    0x31,   {  5,   7, true}, 0xF031, {  5,   7, false}},
        {u8"B",                    0x32,   {  5,   6, true}, 0xF032, {  5,   6, false}},
        {u8"H",                    0x33,   {  4,   7, true}, 0xF033, {  4,   7, false}},
        {u8"G",                    0x34,   {  4,   6, true}, 0xF034, {  4,   6, false}},
        {u8"Y",                    0x35,   {  3,   7, true}, 0xF035, {  3,   7, false}},
        {u8"6",                    0x36,   {  2,   7, true}, 0xF036, {  2,   7, false}},
        {u8"M",                    0x3A,   {  5,   8, true}, 0xF03A, {  5,   8, false}},
        {u8"J",                    0x3B,   {  4,   8, true}, 0xF03B, {  4,   8, false}},
        {u8"U",                    0x3C,   {  3,   8, true}, 0xF03C, {  3,   8, false}},
        {u8"7",                    0x3D,   {  2,   8, true}, 0xF03D, {  2,   8, false}},
        {u8"8",                    0x3E,   {  2,   9, true}, 0xF03E, {  2,   9, false}},
        {u8"Comma",                0x41,   {  5,   9, true}, 0xF041, {  5,   9, false}},
        {u8"K",                    0x42,   {  4,   9, true}, 0xF042, {  4,   9, false}},
        {u8"I",                    0x43,   {  3,   9, true}, 0xF043, {  3,   9, false}},
        {u8"O",                    0x44,   {  3,  10, true}, 0xF044, {  3,  10, false}},
        {u8"0",                    0x45,   {  2,  11, true}, 0xF045, {  2,  11, false}},
        {u8"9",                    0x46,   {  2,  10, true}, 0xF046, {  2,  10, false}},
        {u8"Period",               0x49,   {  5,  10, true}, 0xF049, {  5,  10, false}},
        {u8"Forward Slash",        0x4A,   {  5,  11, true}, 0xF04A, {  5,  11, false}},
        {u8"L",                    0x4B,   {  4,  10, true}, 0xF04B, {  4,  10, false}},
        {u8"Semicolon",            0x4C,   {  4,  11, true}, 0xF04C, {  4,  11, false}},
        {u8"P",                    0x4D,   {  3,  11, true}, 0xF04D, {  3,  11, false}},
        {u8"Minus",                0x4E,   {  2,  12, true}, 0xF04E, {  2,  12, false}},
        {u8"Apostrophe",           0x52,   {  4,  12, true}, 0xF052, {  4,  12, false}},
        {u8"Open Square Bracket",  0x54,   {  3,  12, true}, 0xF054, {  3,  12, false}},
        {u8"Equals",               0x55,   {  2,  13, true}, 0xF055, {  2,  13, false}},
        {u8"Caps Lock",            0x58,   {  4,   1, true}, 0xF058, {  4,   1, false}},
        {u8"Right Shift",          0x59,   {  5,  12, true}, 0xF059, {  5,  12, false}},
        {u8"Enter",                0x5A,   {  4,  13, true}, 0xF05A, {  4,  13, false}},
        {u8"Close Square Bracket", 0x5B,   {  3,  13, true}, 0xF05B, {  3,  13, false}},
        {u8"Backslash",            0x5D,   {  3,  14, true}, 0xF05D, {  3,  14, false}},
        {u8"Backspace",            0x66,   {  2,  14, true}, 0xF066, {  2,  14, false}},
        {u8"Keypad 1",             0x69,   {  5,  14, true}, 0xF069, {  5,  14, false}},
        {u8"Keypad 4",             0x6B,   {  4,  14, true}, 0xF06B, {  4,  14, false}},
        {u8"Keypad 7",             0x6C,   {  3,  18, true}, 0xF06C, {  3,  18, false}},
        {u8"Keypad 0",             0x70,   {  6,  12, true}, 0xF070, {  6,  12, false}},
        {u8"Keypad Period",        0x71,   {  6,  13, true}, 0xF071, {  6,  13, false}},
        {u8"Keypad 2",             0x72,   {  5,  15, true}, 0xF072, {  5,  15, false}},
        {u8"Keypad 5",             0x73,   {  4,  15, true}, 0xF073, {  4,  15, false}},
        {u8"Keypad 6",             0x74,   {  4,  16, true}, 0xF074, {  4,  16, false}},
        {u8"Keypad 8",             0x75,   {  3,  19, true}, 0xF075, {  3,  19, false}},
        {u8"Escape",               0x76,   {  1,   1, true}, 0xF076, {  1,   1, false}},
        {u8"Num Lock",             0x77,   {  2,  18, true}, 0xF077, {  2,  18, false}},
        {u8"F11",                  0x78,   {  1,  12, true}, 0xF078, {  1,  12, false}},
        {u8"Keypad Plus",          0x79,   {  3,  21, true}, 0xF079, {  3,  21, false}},
        {u8"Keypad 3",             0x7A,   {  5,  16, true}, 0xF07A, {  5,  16, false}},
        {u8"Keypad Minus",         0x7B,   {  2,  22, true}, 0xF07B, {  2,  22, false}},
        {u8"Keypad Asterisk",      0x7C,   {  2,  21, true}, 0xF07C, {  2,  21, false}},
        {u8"Keypad 9",             0x7D,   {  3,  20, true}, 0xF07D, {  3,  20, false}},
        {u8"Scroll Lock",          0x7E,   {  1,  15, true}, 0xF07E, {  1,  15, false}},
        {u8"F7",                   0x83,   {  1,   8, true}, 0xF083, {  1,   8, false}},
        {u8"WWW Search",           0xE010, INVALID_KEYCODE, 0xE0F010, INVALID_KEYCODE},
        {u8"Right Alt",            0xE011, {  6,   5, true}, 0xE0F011, {  6,   5, false}},
        {u8"Right Control",        0xE014, {  6,   8, true}, 0xE0F014, {  6,   8, false}},
        {u8"Previous Track",       0xE015, {  1,  18, true}, 0xE0F015, {  1,  18, false}},
        {u8"WWW Favorites",        0xE018, INVALID_KEYCODE, 0xE0F018, INVALID_KEYCODE},
        {u8"Left GUI",             0xE01F, {  6,   2, true}, 0xE0F01F, {  6,   2, false}},
        {u8"WWW Refresh",          0xE020, INVALID_KEYCODE, 0xE0F020, INVALID_KEYCODE},
        {u8"Volume Down",          0xE021, {  0,   4, true}, 0xE0F021, {  0,   4, false}},
        {u8"Mute",                 0xE023, {  0,   3, true}, 0xE0F023, {  0,   3, false}},
        {u8"Right GUI",            0xE027, {  6,   7, true}, 0xE0F027, {  6,   7, false}},
        {u8"WWW Stop",             0xE028, INVALID_KEYCODE, 0xE0F028, INVALID_KEYCODE},
        {u8"Calculator",           0xE02B, INVALID_KEYCODE, 0xE0F02B, INVALID_KEYCODE},
        {u8"Apps",                 0xE02F, {  0,   2, true}, 0xE0F02F, {  0,   2, false}},
        {u8"WWW Forward",          0xE030, INVALID_KEYCODE, 0xE0F030, INVALID_KEYCODE},
        {u8"Volume Up",            0xE032, {  0,   5, true}, 0xE0F032, {  0,   5, false}},
        {u8"Play/Pause",           0xE034, {  1,  19, true}, 0xE0F034, {  1,  19, false}},
        {u8"Power",                0xE037, INVALID_KEYCODE, 0xE0F037, INVALID_KEYCODE},
        {u8"WWW Back",             0xE038, INVALID_KEYCODE, 0xE0F038, INVALID_KEYCODE},
        {u8"WWW Home",             0xE03A, INVALID_KEYCODE, 0xE0F03A, INVALID_KEYCODE},
        {u8"Stop",                 0xE03B, {  1,  17, true}, 0xE0F03B, {  1,  17, false}},
        {u8"Sleep",                0xE03F, INVALID_KEYCODE, 0xE0F03F, INVALID_KEYCODE},
        {u8"My Computer",          0xE040, INVALID_KEYCODE, 0xE0F040, INVALID_KEYCODE},
        {u8"E-Mail",               0xE048, INVALID_KEYCODE, 0xE0F048, INVALID_KEYCODE},
        {u8"Keypad Forward Slash", 0xE04A, {  2,  18, true}, 0xE0F04A, {  2,  18, false}},
        {u8"Next Track",           0xE04D, {  1,  20, true}, 0xE0F04D, {  1,  20, false}},
        {u8"Media Select",         0xE050, INVALID_KEYCODE, 0xE0F050, INVALID_KEYCODE},
        {u8"Keypad Enter",         0xE05A, {  5,  17, true}, 0xE0F05A, {  5,  17, false}},
        {u8"Wake",                 0xE05E, INVALID_KEYCODE, 0xE0F05E, INVALID_KEYCODE},
        {u8"End",                  0xE069, {  3,  16, true}, 0xE0F069, {  3,  16, false}},
        {u8"Cursor Left",          0xE06B, {  6,   9, true}, 0xE0F06B, {  6,   9, false}},
        {u8"Home",                 0xE06C, {  2,  16, true}, 0xE0F06C, {  2,  16, false}},
        {u8"Insert",               0xE070, {  2,  15, true}, 0xE0F070, {  2,  15, false}},
        {u8"Delete",               0xE071, {  3,  15, true}, 0xE0F071, {  3,  15, false}},
        {u8"Cursor Down",          0xE072, {  6,  10, true}, 0xE0F072, {  6,  10, false}},
        {u8"Cursor Right",         0xE074, {  6,  11, true}, 0xE0F074, {  6,  11, false}},
        {u8"Cursor Up",            0xE075, {  5,  13, true}, 0xE0F075, {  5,  13, false}},
        {u8"Page Down",            0xE07A, {  3,  17, true}, 0xE0F07A, {  3,  17, false}},
        {u8"Page Up",              0xE07D, {  2,  17, true}, 0xE0F07D, {  2,  17, false}},
        {u8"Print Screen",         0xE012E07C, {  1,  14, true}, 0xE0F07CE0F012, {  1,  14, false}},
        {u8"Pause",                0xE11477E1F014F077, {  1,  16, true}, 0xE11477E1F014F077, {  1,  16, false}},
    };

    keycode convert(scancode in_code) override;
};

#endif // _KEYBOARD_SCANCODE_SET_2_HPP
