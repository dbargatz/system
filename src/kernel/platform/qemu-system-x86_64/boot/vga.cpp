#include "vga.hpp"

using kernel::platform::x86_64::vga;

vga::vga() {
    clear_screen(color::black);
}

void vga::clear_row(const color in_color, const uint8_t in_row) {
    auto old_row = _cur_row;
    auto old_col = _cur_column;
    set_position(in_row, 0);

    // Write spaces for the entire row, using the same color for the foreground
    // and background.
    for (auto i = 0; i < _max_columns; i++) {
        write((unsigned char *)" ", in_color, in_color);
    }

    set_position(old_row, old_col);
}

void vga::clear_screen(const color in_color) {
    for (auto i = 0; i < _max_rows; i++) {
        clear_row(in_color, i);
    }
}

void vga::scroll() {
    auto bytes_per_row = sizeof(uint16_t) * _max_columns;
    auto dst = (uint16_t *)(_base_address);
    auto src = dst + (bytes_per_row);
    auto end = dst + (bytes_per_row * (_max_rows - 1));

    while (dst != end) {
        *dst++ = *src++;
    }
}

void vga::set_position(const uint8_t in_row, const uint8_t in_column) {
    _cur_row = in_row < _max_rows ? in_row : _max_rows - 1;
    _cur_column = in_column < _max_columns ? in_column : _max_columns - 1;
}

void vga::write(const void * in_string, const color in_text_color, 
    const color in_bg_color) {
    uint16_t color = ((uint8_t)in_bg_color << 4 | (uint8_t)in_text_color);
    uint8_t * cur_char = (uint8_t *)in_string;

    while ('\0' != *cur_char) {
        // Move to the next line (row) if we see a newline or are past
        // the screen width.
        if ('\n' == *cur_char || _cur_column >= _max_columns) {
            _cur_row++;
            if (_cur_row >= _max_rows) { 
                scroll(); 
            }
            _cur_column = 0;
        }

        if ('\n' != *cur_char) {
            auto byte_offset = (_cur_row * sizeof(uint16_t) * _max_columns);
            byte_offset += (sizeof(uint16_t) * _cur_column);
            uint16_t * addr = (uint16_t *)(_base_address + byte_offset);
            *addr = color << 8 | *cur_char;
            _cur_column++;
        }
        
        cur_char++;
    }
}
