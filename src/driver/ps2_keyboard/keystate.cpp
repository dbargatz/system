#include "keystate.hpp"
#include <cstring>

keystate::keystate() {
    std::memset(_state, 0, sizeof(_state));
}

std::string keystate::set_key(keycode& in_code) {
    // Update the state array.
    auto col_major_idx = in_code.column / 64;
    auto col_minor_idx = in_code.column % 64;
    auto col_block = _state[in_code.row][col_major_idx];
    if(in_code.pressed) {
        col_block |= (1 << col_minor_idx);
    } else {
        col_block &= ~(1 << col_minor_idx);
    }
    _state[in_code.row][col_major_idx] = col_block;

    // If this is a modifier or toggle (lock) key, update the state flags.
    if(in_code.row == 1 && in_code.column == 15 && in_code.pressed) {
        _scroll_lock = !_scroll_lock;
    } else if(in_code.row == 2 && in_code.column == 18 && in_code.pressed) {
        _num_lock = !_num_lock;
    } else if(in_code.row == 4 && in_code.column == 1 && in_code.pressed) {
        _caps_lock = !_caps_lock;
    } else if(in_code.row == 5 && (in_code.column == 1 || in_code.column == 12)) {
        _shift = in_code.pressed;
    } else if(in_code.row == 6 && (in_code.column == 1 || in_code.column == 8)) {
        _ctrl = in_code.pressed;
    } else if(in_code.row == 6 && (in_code.column == 3 || in_code.column == 5)) {
        _alt = in_code.pressed;
    }

    // If this key should generate a unicode character, return a text object
    // with the unicode; otherwise, return an empty text. Only generate unicode
    // on key press, not release.
    if(!in_code.pressed) {
        return u8"";
    }
    switch(in_code.row) {
        case 2:
            switch(in_code.column) {
                case 1:  return (_shift ? u8"~" : u8"`");
                case 2:  return (_shift ? u8"!" : u8"1");
                case 3:  return (_shift ? u8"@" : u8"2");
                case 4:  return (_shift ? u8"#" : u8"3");
                case 5:  return (_shift ? u8"$" : u8"4");
                case 6:  return (_shift ? u8"%" : u8"5");
                case 7:  return (_shift ? u8"^" : u8"6");
                case 8:  return (_shift ? u8"&" : u8"7");
                case 9:  return (_shift ? u8"*" : u8"8");
                case 10: return (_shift ? u8"(" : u8"9");
                case 11: return (_shift ? u8")" : u8"0");
                case 12: return (_shift ? u8"_" : u8"-");
                case 13: return (_shift ? u8"+" : u8"=");
                case 19: return u8"/";
                case 20: return u8"*";
                case 21: return u8"-";
                default: return u8"";
            }
        case 3:
            switch(in_code.column) {
                case 1:  return u8"\t";
                case 2:  return (_shift ^ _caps_lock ? u8"Q" : u8"q");
                case 3:  return (_shift ^ _caps_lock ? u8"W" : u8"w");
                case 4:  return (_shift ^ _caps_lock ? u8"E" : u8"e");
                case 5:  return (_shift ^ _caps_lock ? u8"R" : u8"r");
                case 6:  return (_shift ^ _caps_lock ? u8"T" : u8"t");
                case 7:  return (_shift ^ _caps_lock ? u8"Y" : u8"y");
                case 8:  return (_shift ^ _caps_lock ? u8"U" : u8"u");
                case 9:  return (_shift ^ _caps_lock ? u8"I" : u8"i");
                case 10: return (_shift ^ _caps_lock ? u8"O" : u8"o");
                case 11: return (_shift ^ _caps_lock ? u8"P" : u8"p");
                case 12: return (_shift ^ _caps_lock ? u8"{" : u8"[");
                case 13: return (_shift ^ _caps_lock ? u8"}" : u8"]");
                case 14: return (_shift ^ _caps_lock ? u8"|" : u8"\\");
                case 18: return (_num_lock ? u8"7" : u8"");
                case 19: return (_num_lock ? u8"8" : u8"");
                case 20: return (_num_lock ? u8"9" : u8"");
                case 21: return u8"+";
                default: return u8"";
            }
        case 4:
            switch(in_code.column) {
                case 2:  return (_shift ^ _caps_lock ? u8"A"  : u8"a");
                case 3:  return (_shift ^ _caps_lock ? u8"S"  : u8"s");
                case 4:  return (_shift ^ _caps_lock ? u8"D"  : u8"d");
                case 5:  return (_shift ^ _caps_lock ? u8"F"  : u8"f");
                case 6:  return (_shift ^ _caps_lock ? u8"G"  : u8"g");
                case 7:  return (_shift ^ _caps_lock ? u8"H"  : u8"h");
                case 8:  return (_shift ^ _caps_lock ? u8"J"  : u8"j");
                case 9:  return (_shift ^ _caps_lock ? u8"K"  : u8"k");
                case 10: return (_shift ^ _caps_lock ? u8"L"  : u8"l");
                case 11: return (_shift ^ _caps_lock ? u8":"  : u8";");
                case 12: return (_shift ^ _caps_lock ? u8"\"" : u8"'");
                case 13: return u8"\n";
                case 14: return (_num_lock ? u8"4" : u8"");
                case 15: return (_num_lock ? u8"5" : u8"");
                case 16: return (_num_lock ? u8"6" : u8"");
                default: return u8"";
            }
        case 5:
            switch(in_code.column) {
                case 2:  return (_shift ^ _caps_lock ? u8"Z" : u8"z");
                case 3:  return (_shift ^ _caps_lock ? u8"X" : u8"x");
                case 4:  return (_shift ^ _caps_lock ? u8"C" : u8"c");
                case 5:  return (_shift ^ _caps_lock ? u8"V" : u8"v");
                case 6:  return (_shift ^ _caps_lock ? u8"B" : u8"b");
                case 7:  return (_shift ^ _caps_lock ? u8"N" : u8"n");
                case 8:  return (_shift ^ _caps_lock ? u8"M" : u8"m");
                case 9:  return (_shift ^ _caps_lock ? u8"<" : u8",");
                case 10: return (_shift ^ _caps_lock ? u8">" : u8".");
                case 11: return (_shift ^ _caps_lock ? u8"?" : u8"/");
                case 14: return (_num_lock ? u8"1" : u8"");
                case 15: return (_num_lock ? u8"2" : u8"");
                case 16: return (_num_lock ? u8"3" : u8"");
                case 17: return u8"\n";
                default: return u8"";
            }
        case 6:
            switch(in_code.column) {
                case 4:  return u8" ";
                case 12: return (_num_lock ? u8"0" : u8"");
                case 13: return (_num_lock ? u8"." : u8"");
                default: return u8"";
            }
        default:
            return u8"";
    }
}
