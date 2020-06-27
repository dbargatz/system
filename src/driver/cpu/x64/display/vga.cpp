#include "vga.hpp"
#include "../std/memcpy.hpp"

vga::vga() : _cur_column(0), _cur_row(0) {
    clear_screen(color::black);
}

void vga::clear_row(const color in_color, const uint8_t in_row) {
    auto old_row = _cur_row;
    auto old_col = _cur_column;
    set_position(in_row, 0);

    // Write spaces for the entire row, using the same color for the foreground
    // and background.
    for (auto i = 0; i < _MAX_COLUMNS; i++) {
        write((unsigned char *)" ", in_color, in_color);
    }

    set_position(old_row, old_col);
}

void vga::clear_screen(const color in_color) {
    for (auto i = 0; i < _MAX_ROWS; i++) {
        clear_row(in_color, i);
    }
}

void vga::scroll() {
    auto dst = (uint8_t *)_START_ADDRESS;
    auto src = dst + _ROW_LENGTH;
    _cur_row = 0;

    while (dst <= (uint8_t *)_END_ADDRESS) {
        clear_row(color::black, _cur_row);
        memcpy(dst, src, _ROW_LENGTH);
        dst += _ROW_LENGTH;
        src += _ROW_LENGTH;
        _cur_row++;
    }
    _cur_column = 0;
}

void vga::set_position(const uint8_t in_row, const uint8_t in_column) {
    _cur_row = in_row < _MAX_ROWS ? in_row : _MAX_ROWS - 1;
    _cur_column = in_column < _MAX_COLUMNS ? in_column : _MAX_COLUMNS - 1;
}

void vga::write(const void * in_string, const color in_text_color, 
    const color in_bg_color) {
    uint16_t color = ((uint8_t)in_bg_color << 4 | (uint8_t)in_text_color);
    uint8_t * cur_char = (uint8_t *)in_string;

    while ('\0' != *cur_char) {
        // Move to the next line (row) if we see a newline or are past
        // the screen width.
        if ('\n' == *cur_char || _cur_column >= _MAX_COLUMNS) {
            _cur_row++;
            if (_cur_row >= _MAX_ROWS) {
                scroll();
            }
            _cur_column = 0;

            if('\n' == *cur_char) {
                cur_char++;
                continue;
            }
        }

        auto byte_offset = (_cur_row * _ROW_LENGTH);
        byte_offset += (sizeof(uint16_t) * _cur_column);
        uint16_t * addr = (uint16_t *)(_START_ADDRESS + byte_offset);
        *addr = color << 8 | *cur_char;
        _cur_column++;
        cur_char++;
    }
}
