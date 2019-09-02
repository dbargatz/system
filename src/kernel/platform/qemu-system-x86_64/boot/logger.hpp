#include "vga.hpp"

namespace kernel::platform::x86_64 {
    class logger {
    public:
        logger();

        void dbg(const char * in_msg, ...);
        void err(const char * in_msg, ...);
        void inf(const char * in_msg, ...);
        void wrn(const char * in_msg, ...);

    private:
        static vga _vga;

        static void write(const vga::color in_text_color, 
            const vga::color in_bg_color, const char * in_prefix, 
            const char * in_msg, __builtin_va_list in_varargs);
    };
};