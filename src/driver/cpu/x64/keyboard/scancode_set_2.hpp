#ifndef _KEYBOARD_SCANCODE_SET_2_HPP
#define _KEYBOARD_SCANCODE_SET_2_HPP

#include "scancode_set.hpp"

class scancode_set_2 : public scancode_set {
public:
    struct scancode_to_keycode mappings[125] = {
        {"F9",                   0x01,   0xF001,    {1, 10}},
        {"F5",                   0x03,   0xF003,    {1, 6}},
        {"F3",                   0x04,   0xF004,    {1, 4}},
        {"F1",                   0x05,   0xF005,    {1, 2}},
        {"F2",                   0x06,   0xF006,    {1, 3}},
        {"F12",                  0x07,   0xF007,    {1, 13}},
        {"F10",                  0x09,   0xF009,    {1, 11}},
        {"F8",                   0x0A,   0xF00A,    {1, 9}},
        {"F6",                   0x0B,   0xF00B,    {1, 7}},
        {"F4",                   0x0C,   0xF00C,    {1, 5}},
        {"Tab",                  0x0D,   0xF00D,    {3, 1}},
        {"Backtick",             0x0E,   0xF00E,    {2, 1}},
        {"Left Alt",             0x11,   0xF011,    {6, 3}},
        {"Left Shift",           0x12,   0xF012,    {5, 1}},
        {"Left Control",         0x14,   0xF014,    {6, 1}},
        {"Q",                    0x15,   0xF015,    {3, 2}},
        {"1",                    0x16,   0xF016,    {2, 2}},
        {"Z",                    0x1A,   0xF01A,    {5, 2}},
        {"S",                    0x1B,   0xF01B,    {4, 3}},
        {"A",                    0x1C,   0xF01C,    {4, 2}},
        {"W",                    0x1D,   0xF01D,    {3, 3}},
        {"2",                    0x1E,   0xF01E,    {2, 3}},
        {"C",                    0x21,   0xF021,    {5, 4}},
        {"X",                    0x22,   0xF022,    {5, 3}},
        {"D",                    0x23,   0xF023,    {4, 4}},
        {"E",                    0x24,   0xF024,    {0, 0}}, // TODO: Finish row/col mappings
        {"4",                    0x25,   0xF025,    {0, 0}},
        {"3",                    0x26,   0xF026,    {0, 0}},
        {"Space",                0x29,   0xF029,    {0, 0}},
        {"V",                    0x2A,   0xF02A,    {0, 0}},
        {"F",                    0x2B,   0xF02B,    {0, 0}},
        {"T",                    0x2C,   0xF02C,    {0, 0}},
        {"R",                    0x2D,   0xF02D,    {0, 0}},
        {"5",                    0x2E,   0xF02E,    {0, 0}},
        {"N",                    0x31,   0xF031,    {0, 0}},
        {"B",                    0x32,   0xF032,    {0, 0}},
        {"H",                    0x33,   0xF033,    {0, 0}},
        {"G",                    0x34,   0xF034,    {0, 0}},
        {"Y",                    0x35,   0xF035,    {0, 0}},
        {"6",                    0x36,   0xF036,    {0, 0}},
        {"M",                    0x3A,   0xF03A,    {0, 0}},
        {"J",                    0x3B,   0xF03B,    {0, 0}},
        {"U",                    0x3C,   0xF03C,    {0, 0}},
        {"7",                    0x3D,   0xF03D,    {0, 0}},
        {"8",                    0x3E,   0xF03E,    {0, 0}},
        {"Comma",                0x41,   0xF041,    {0, 0}},
        {"K",                    0x42,   0xF042,    {0, 0}},
        {"I",                    0x43,   0xF043,    {0, 0}},
        {"O",                    0x44,   0xF044,    {0, 0}},
        {"0",                    0x45,   0xF045,    {0, 0}},
        {"9",                    0x46,   0xF046,    {0, 0}},
        {"Period",               0x49,   0xF049,    {0, 0}},
        {"Forward Slash",        0x4A,   0xF04A,    {0, 0}},
        {"L",                    0x4B,   0xF04B,    {0, 0}},
        {"Semicolon",            0x4C,   0xF04C,    {0, 0}},
        {"P",                    0x4D,   0xF04D,    {0, 0}},
        {"Minus",                0x4E,   0xF04E,    {0, 0}},
        {"Apostrophe",           0x52,   0xF052,    {0, 0}},
        {"Open Square Bracket",  0x54,   0xF054,    {0, 0}},
        {"Equals",               0x55,   0xF055,    {0, 0}},
        {"Caps Lock",            0x58,   0xF058,    {4, 1}},
        {"Right Shift",          0x59,   0xF059,    {0, 0}},
        {"Enter",                0x5A,   0xF05A,    {0, 0}},
        {"Close Square Bracket", 0x5B,   0xF05B,    {0, 0}},
        {"Backslash",            0x5D,   0xF05D,    {0, 0}},
        {"Backspace",            0x66,   0xF066,    {0, 0}},
        {"Keypad 1",             0x69,   0xF069,    {0, 0}},
        {"Keypad 4",             0x6B,   0xF06B,    {0, 0}},
        {"Keypad 7",             0x6C,   0xF06C,    {0, 0}},
        {"Keypad 0",             0x70,   0xF070,    {0, 0}},
        {"Keypad Period",        0x71,   0xF071,    {0, 0}},
        {"Keypad 2",             0x72,   0xF072,    {0, 0}},
        {"Keypad 5",             0x73,   0xF073,    {0, 0}},
        {"Keypad 6",             0x74,   0xF074,    {0, 0}},
        {"Keypad 8",             0x75,   0xF075,    {0, 0}},
        {"Escape",               0x76,   0xF076,    {0, 0}},
        {"Num Lock",             0x77,   0xF077,    {0, 0}},
        {"F11",                  0x78,   0xF078,    {0, 0}},
        {"Keypad Plus",          0x79,   0xF079,    {0, 0}},
        {"Keypad 3",             0x7A,   0xF07A,    {0, 0}},
        {"Keypad Minus",         0x7B,   0xF07B,    {0, 0}},
        {"Keypad Asterisk",      0x7C,   0xF07C,    {0, 0}},
        {"Keypad 9",             0x7D,   0xF07D,    {0, 0}},
        {"Scroll Lock",          0x7E,   0xF07E,    {0, 0}},
        {"F7",                   0x83,   0xF083,    {0, 0}},
        {"WWW Search",           0xE010, 0xE0F010,  {0, 0}},
        {"Right Alt",            0xE011, 0xE0F011,  {0, 0}},
        {"Right Control",        0xE014, 0xE0F014,  {0, 0}},
        {"Previous Track",       0xE015, 0xE0F015,  {0, 0}},
        {"WWW Favorites",        0xE018, 0xE0F018,  {0, 0}},
        {"Left GUI",             0xE01F, 0xE0F01F,  {0, 0}},
        {"WWW Refresh",          0xE020, 0xE0F020,  {0, 0}},
        {"Volume Down",          0xE021, 0xE0F021,  {0, 0}},
        {"Mute",                 0xE023, 0xE0F023,  {0, 0}},
        {"Right GUI",            0xE027, 0xE0F027,  {0, 0}},
        {"WWW Stop",             0xE028, 0xE0F028,  {0, 0}},
        {"Calculator",           0xE02B, 0xE0F02B,  {0, 0}},
        {"Apps",                 0xE02F, 0xE0F02F,  {0, 0}},
        {"WWW Forward",          0xE030, 0xE0F030,  {0, 0}},
        {"Volume Up",            0xE032, 0xE0F032,  {0, 0}},
        {"Play/Pause",           0xE034, 0xE0F034,  {0, 0}},
        {"Power",                0xE037, 0xE0F037,  {0, 0}},
        {"WWW Back",             0xE038, 0xE0F038,  {0, 0}},
        {"WWW Home",             0xE03A, 0xE0F03A,  {0, 0}},
        {"Stop",                 0xE03B, 0xE0F03B,  {0, 0}},
        {"Sleep",                0xE03F, 0xE0F03F,  {0, 0}},
        {"My Computer",          0xE040, 0xE0F040,  {0, 0}},
        {"E-Mail",               0xE048, 0xE0F048,  {0, 0}},
        {"Keypad Forward Slash", 0xE04A, 0xE0F04A,  {0, 0}},
        {"Next Track",           0xE04D, 0xE0F04D,  {0, 0}},
        {"Media Select",         0xE050, 0xE0F050,  {0, 0}},
        {"Keypad Enter",         0xE05A, 0xE0F05A,  {0, 0}},
        {"Wake",                 0xE05E, 0xE0F05E,  {0, 0}},
        {"End",                  0xE069, 0xE0F069,  {0, 0}},
        {"Cursor Left",          0xE06B, 0xE0F06B,  {0, 0}},
        {"Home",                 0xE06C, 0xE0F06C,  {0, 0}},
        {"Insert",               0xE070, 0xE0F070,  {0, 0}},
        {"Delete",               0xE071, 0xE0F071,  {0, 0}},
        {"Cursor Down",          0xE072, 0xE0F072,  {0, 0}},
        {"Cursor Right",         0xE074, 0xE0F074,  {0, 0}},
        {"Cursor Up",            0xE075, 0xE0F075,  {0, 0}},
        {"Page Down",            0xE07A, 0xE0F07A,  {0, 0}},
        {"Page Up",              0xE07D, 0xE0F07D,  {0, 0}},
        {"Print Screen",         0xE012E07C, 0xE0F07CE0F012, {0, 0}},
        {"Pause",                0xE11477E1F014F077, 0xE11477E1F014F077, {0, 0}},
    };

    keycode convert(scancode in_code) override;
};

#endif // _KEYBOARD_SCANCODE_SET_2_HPP
