#ifndef _DISPLAY_VGA_HPP
#define _DISPLAY_VGA_HPP

#include "../std/stdint.h"

/**
 * Simple kernel-mode implementation of a standard VGA text-mode console with
 * 16 colors, 80 columns, and 25 characters.
 *
 * This is known as VGA Mode 3.
 * 
 * @todo: This class needs to be converted into a userspace driver.
 * @see https://en.wikipedia.org/wiki/VGA_text_mode
 */
class vga {
public:
    /**
     * Standard 16-color CGA palette of colors.
     */
    enum class color : uint8_t {
        black          = 0x0,
        blue           = 0x1,
        green          = 0x2,
        cyan           = 0x3,
        red            = 0x4,
        magenta        = 0x5,
        brown          = 0x6,
        gray           = 0x7,
        dark_gray      = 0x8,
        bright_blue    = 0x9,
        bright_green   = 0xA,
        bright_cyan    = 0xB,
        bright_red     = 0xC,
        bright_magenta = 0xD,
        yellow         = 0xE,
        white          = 0xF,
    };

    vga();
    void clear_row(const color in_color, const uint8_t in_row);
    void clear_screen(const color in_color);
    void scroll();
    void set_position(const uint8_t in_row, const uint8_t in_column);
    void write(const void * in_string, const color in_text_color, 
        const color in_bg_color);

private:
    ///< Fixed number of rows that can be displayed at one time.
    constexpr static const uint8_t MAX_ROWS     = 25;

    ///< Fixed number of columns that can be displayed in a single row.
    constexpr static const uint8_t MAX_COLUMNS  = 80;

    ///< Physical memory address of video RAM.
    ///< @todo Represent with physical address type, not uint64_t.
    static const uint64_t BASE_ADDRESS = (uint64_t)0xB8000;

    ///< The current horizontal position of the cursor on the screen.
    uint8_t _cur_column = 0;

    ///< The current vertical position of the cursor on the screen.
    uint8_t _cur_row = 0;
};

#endif // _DISPLAY_VGA_HPP
