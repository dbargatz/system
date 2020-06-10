#ifndef KERNEL_PLATFORM_X86_64_VGA
#define KERNEL_PLATFORM_X86_64_VGA

#include "../../../../driver/cpu/x64/std/stdint.h"

namespace kernel::platform::x86_64 {
    class vga {
    public:
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
        static const uint8_t _max_rows = 25;
        static const uint8_t _max_columns = 80;
        static const uint64_t _base_address = (uint64_t)0xB8000;

        uint8_t _cur_column = 0;
        uint8_t _cur_row = 0;
    };
};

#endif