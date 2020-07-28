#ifndef _DISPLAY_VGA_HPP
#define _DISPLAY_VGA_HPP

#include <cstdint>
#include <string>

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
    enum class color : std::uint8_t {
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
    void clear_row(const color in_color, const std::uint8_t in_row);
    void clear_screen(const color in_color);
    void scroll();
    void set_position(const std::uint8_t in_row, const std::uint8_t in_column);
    void write(const std::string& in_string, const color in_fg_color,
        const color in_bg_color);

private:
    ///< First physical memory address of video RAM.
    ///< @todo Represent with physical address type, not uint64_t.
    constexpr static const std::uint64_t _START_ADDRESS = (std::uint64_t)0xB8000;

    ///< Fixed number of rows that can be displayed at one time.
    constexpr static const std::uint8_t _MAX_ROWS = 25;

    ///< Fixed number of columns that can be displayed in a single row.
    constexpr static const std::uint8_t _MAX_COLUMNS = 80;

    ///< Length (in bytes) of a complete row of text in memory. Also known as
    ///< the stride.
    constexpr static const std::uint8_t _ROW_LENGTH = _MAX_COLUMNS * sizeof(std::uint16_t);

    ///< Last valid physical memory address of video RAM.
    ///< @todo Represent with physical address type, not uint64_t.
    constexpr static const std::uint64_t _END_ADDRESS = _START_ADDRESS + (_ROW_LENGTH * _MAX_ROWS);

    ///< The current horizontal position of the cursor on the screen. Column
    ///< numbers range from 0 to (MAX_COLUMNS - 1).
    std::uint8_t _cur_column;

    ///< The current vertical position of the cursor on the screen. Row numbers
    ///< range from 0 to (MAX_ROWS - 1).
    std::uint8_t _cur_row;
};

#endif // _DISPLAY_VGA_HPP
