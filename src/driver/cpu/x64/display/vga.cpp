#include "vga.hpp"
#include <cstring.hpp>

vga::vga() : _cur_column(0), _cur_row(0) {
    clear_screen(color::black);
}

void vga::clear_row(const color in_color, const std::uint8_t in_row) {
    auto old_row = _cur_row;
    auto old_col = _cur_column;
    set_position(in_row, 0);

    // Write spaces for the entire row, using the same color for the foreground
    // and background.
    for (auto i = 0; i < _MAX_COLUMNS; i++) {
        write(u8" ", in_color, in_color);
    }

    set_position(old_row, old_col);
}

void vga::clear_screen(const color in_color) {
    for (auto i = 0; i < _MAX_ROWS; i++) {
        clear_row(in_color, i);
    }
}

void vga::scroll() {
    auto dst = (std::uint8_t *)_START_ADDRESS;
    auto src = dst + _ROW_LENGTH;
    _cur_row = 0;

    while (dst < (std::uint8_t *)_END_ADDRESS) {
        clear_row(color::black, _cur_row);
        std::memcpy(dst, src, _ROW_LENGTH);
        dst += _ROW_LENGTH;
        src += _ROW_LENGTH;
        _cur_row++;
    }
    clear_row(color::black, _cur_row);
}

void vga::set_position(const std::uint8_t in_row, const std::uint8_t in_column) {
    _cur_row = in_row < _MAX_ROWS ? in_row : _MAX_ROWS - 1;
    _cur_column = in_column < _MAX_COLUMNS ? in_column : _MAX_COLUMNS - 1;
}

void vga::write(const std::string& in_string, const color in_fg_color,
    const color in_bg_color) {
    auto cur_char = in_string.c_str();
    std::uint16_t color = ((std::uint8_t)in_bg_color << 4 | (std::uint8_t)in_fg_color);

    auto addr = (std::uint16_t *)(_START_ADDRESS + (_cur_row * _ROW_LENGTH));
    addr += _cur_column;

    while ('\0' != *cur_char) {
        // On newlines, pad out the remainder of the line with spaces to fill in
        // the background color and advance the column to max, then move to the
        // next char.
        if('\n' == *cur_char) {
            while(_cur_column++ < _MAX_COLUMNS) {
                *addr++ = color << 8 | ' ';
            }
            cur_char++;
            continue;
        }

        // If we're at the end of the line, wrap the line, scrolling if needed.
        if(_cur_column >= _MAX_COLUMNS) {
            _cur_column = 0;
            _cur_row++;
            if (_cur_row >= _MAX_ROWS) {
                scroll();
            }
            addr = (std::uint16_t *)(_START_ADDRESS + (_cur_row * _ROW_LENGTH));
        }

        // Write the styled character to the buffer and move to the next.
        *addr++ = color << 8 | *cur_char;
        _cur_column++;
        cur_char++;
    }
}
