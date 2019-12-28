#ifndef KERNEL_PLATFORM_X86_64_TYPES_TEXT
#define KERNEL_PLATFORM_X86_64_TYPES_TEXT

namespace kernel::platform::x86_64::types {
    typedef signed char int8_t;
    typedef signed short int16_t;
    typedef signed int int32_t;
    typedef signed long long int64_t;

    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int uint32_t;
    typedef unsigned long long uint64_t;

    typedef float float32_t;
    typedef double float64_t;

    typedef uint64_t size_t;

    class text {
    public:
        template<typename ...Args>
        text(const char * in_format_str, const Args&&... in_args) {
            _length_in_chars = 0;
            format(in_format_str, in_args...);
        }

        const char * get() { return (const char *)_buf; }

    private:
        char _buf[256];
        size_t _length_in_chars;

        void format(const char * in_format_str);

        template<typename T, typename ...RemainingArgs>
        void format(const char * in_format_str, const T& in_arg, RemainingArgs&&... in_remaining_args) {
            while('\0' != *in_format_str) {
                if('{' == *in_format_str) {
                    ++in_format_str;
                    if('{' != *in_format_str) {
                        // TODO: handle positional index

                        // TODO: handle colon
                        if(':' == *in_format_str) {
                            ++in_format_str;
                        }

                        // TODO: handle padding alignment
                        // TODO: handle padding char
                        // TODO: handle padding width
                        // TODO: handle prefix (#)

                        // TODO: handle argument format (x, X, d, s, etc)
                        if('d' == *in_format_str) {
                            format_arg(in_arg);
                            ++in_format_str;
                        }
                        else if('s' == *in_format_str) {
                            format_arg(in_arg, 0, false);
                            ++in_format_str;
                        }
                        else if('x' == *in_format_str) {
                            format_arg(in_arg, 16, false);
                            ++in_format_str;
                        }
                        else if('X' == *in_format_str) {
                            format_arg(in_arg, 16, true);
                            ++in_format_str;
                        }
                        else {
                            // Format argument using defaults. Do not advance.
                            format_arg(in_arg);
                        }

                        if('}' != *in_format_str) {
                            // TODO: panic/throw (next expected char was closing brace)
                            _buf[0] = 'X';
                            _buf[1] = 'X';
                            _buf[2] = 'X';
                            _buf[3] = ':';
                            _buf[4] = ' ';
                            _buf[5] = *in_format_str;
                            _length_in_chars = 6;  
                            return;                  
                        }

                        format(++in_format_str, in_remaining_args...);
                        return;
                    }
                }
                
                // TODO: bounds-check _buf
                _buf[_length_in_chars++] = *in_format_str++;
            }

            // TODO: panic/throw (more args provided than there were format args)
        }

        template<typename T>
        void format_arg(const T in_arg, uint8_t in_base = 10, bool in_uppercase_digits = false);
    };
};

#endif //KERNEL_PLATFORM_X86_64_TYPES_TEXT