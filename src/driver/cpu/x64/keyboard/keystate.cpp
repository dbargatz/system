#include "keystate.hpp"
#include "../std/memset.hpp"

keystate::keystate() {
    memset(_state, 0, sizeof(_state));
}

text keystate::set_key(keycode& in_code) {
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
        return "";
    }
    switch(in_code.row) {
        case 2:
            switch(in_code.column) {
                case 1:  return (_shift ? "~" : "`");
                case 2:  return (_shift ? "!" : "1");
                case 3:  return (_shift ? "@" : "2");
                case 4:  return (_shift ? "#" : "3");
                case 5:  return (_shift ? "$" : "4");
                case 6:  return (_shift ? "%" : "5");
                case 7:  return (_shift ? "^" : "6");
                case 8:  return (_shift ? "&" : "7");
                case 9:  return (_shift ? "*" : "8");
                case 10: return (_shift ? "(" : "9");
                case 11: return (_shift ? ")" : "0");
                case 12: return (_shift ? "_" : "-");
                case 13: return (_shift ? "+" : "=");
                case 19: return "/";
                case 20: return "*";
                case 21: return "-";
                default: return "";
            }
        case 3:
            switch(in_code.column) {
                case 1:  return "\t";
                case 2:  return (_shift ^ _caps_lock ? "Q" : "q");
                case 3:  return (_shift ^ _caps_lock ? "W" : "w");
                case 4:  return (_shift ^ _caps_lock ? "E" : "e");
                case 5:  return (_shift ^ _caps_lock ? "R" : "r");
                case 6:  return (_shift ^ _caps_lock ? "T" : "t");
                case 7:  return (_shift ^ _caps_lock ? "Y" : "y");
                case 8:  return (_shift ^ _caps_lock ? "U" : "u");
                case 9:  return (_shift ^ _caps_lock ? "I" : "i");
                case 10: return (_shift ^ _caps_lock ? "O" : "o");
                case 11: return (_shift ^ _caps_lock ? "P" : "p");
                case 12: return (_shift ^ _caps_lock ? "{" : "[");
                case 13: return (_shift ^ _caps_lock ? "}" : "]");
                case 14: return (_shift ^ _caps_lock ? "|" : "\\");
                case 18: return (_num_lock ? "7" : "");
                case 19: return (_num_lock ? "8" : "");
                case 20: return (_num_lock ? "9" : "");
                case 21: return "+";
                default: return "";
            }
        case 4:
            switch(in_code.column) {
                case 2:  return (_shift ^ _caps_lock ? "A" : "a");
                case 3:  return (_shift ^ _caps_lock ? "S" : "s");
                case 4:  return (_shift ^ _caps_lock ? "D" : "d");
                case 5:  return (_shift ^ _caps_lock ? "F" : "f");
                case 6:  return (_shift ^ _caps_lock ? "G" : "g");
                case 7:  return (_shift ^ _caps_lock ? "H" : "h");
                case 8:  return (_shift ^ _caps_lock ? "J" : "j");
                case 9:  return (_shift ^ _caps_lock ? "K" : "k");
                case 10: return (_shift ^ _caps_lock ? "L" : "l");
                case 11: return (_shift ^ _caps_lock ? ":" : ";");
                case 12: return (_shift ^ _caps_lock ? "\"" : "'");
                case 13: return "\n";
                case 14: return (_num_lock ? "4" : "");
                case 15: return (_num_lock ? "5" : "");
                case 16: return (_num_lock ? "6" : "");
                default: return "";
            }
        case 5:
            switch(in_code.column) {
                case 2:  return (_shift ^ _caps_lock ? "Z" : "z");
                case 3:  return (_shift ^ _caps_lock ? "X" : "x");
                case 4:  return (_shift ^ _caps_lock ? "C" : "c");
                case 5:  return (_shift ^ _caps_lock ? "V" : "v");
                case 6:  return (_shift ^ _caps_lock ? "B" : "b");
                case 7:  return (_shift ^ _caps_lock ? "N" : "n");
                case 8:  return (_shift ^ _caps_lock ? "M" : "m");
                case 9:  return (_shift ^ _caps_lock ? "<" : ",");
                case 10: return (_shift ^ _caps_lock ? ">" : ".");
                case 11: return (_shift ^ _caps_lock ? "?" : "/");
                case 14: return (_num_lock ? "1" : "");
                case 15: return (_num_lock ? "2" : "");
                case 16: return (_num_lock ? "3" : "");
                case 17: return "\n";
                default: return "";
            }
        case 6:
            switch(in_code.column) {
                case 4:  return " ";
                case 12: return (_num_lock ? "0" : "");
                case 13: return (_num_lock ? "." : "");
                default: return "";
            }
        default:
            return "";
    }
}
