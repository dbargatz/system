#ifndef KERNEL_PLATFORM_X86_64_TYPES_TEXT
#define KERNEL_PLATFORM_X86_64_TYPES_TEXT

#include "stdint.h"

namespace kernel::platform::x86_64::types {
    class text {
    public:
        template<typename ...Args>
        text(const char * in_format_str, Args&&... in_args) {
            _length_in_chars = 0;
            format(in_format_str, static_cast<Args>(in_args)...);
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
                    
                    if('\0' == *in_format_str) {
                        // TODO: panic/throw (single open brace at end of string)
                        return;
                    }
                    else if('{' != *in_format_str) {
                        bool prepend_prefix = false;

                        // TODO: handle positional index (must be followed by colon if any of the below fields are specified)

                        // TODO: handle padding char
                        // TODO: handle padding alignment
                        // TODO: handle sign
                        
                        // Handle the "alternate form" option, if present.
                        if('#' == *in_format_str) {
                            prepend_prefix = true;
                            ++in_format_str;
                        }

                        // TODO: handle padding width

                        // TODO: handle argument format (x, X, d, s, etc)
                        if('d' == *in_format_str) {
                            format_arg(in_arg, 10, false, prepend_prefix);
                            ++in_format_str;
                        }
                        else if('s' == *in_format_str) {
                            format_arg(in_arg, 0, false, prepend_prefix);
                            ++in_format_str;
                        }
                        else if('x' == *in_format_str) {
                            format_arg(in_arg, 16, false, prepend_prefix);
                            ++in_format_str;
                        }
                        else if('X' == *in_format_str) {
                            format_arg(in_arg, 16, true, prepend_prefix);
                            ++in_format_str;
                        }
                        else {
                            // Format argument using defaults. Do not advance.
                            format_arg(in_arg);
                        }

                        if('}' != *in_format_str++) {
                            // TODO: panic/throw (next expected char was closing brace)
                            return;                  
                        }

                        format(in_format_str, in_remaining_args...);
                        return;
                    }
                }
                
                // TODO: bounds-check _buf
                _buf[_length_in_chars++] = *in_format_str++;
            }

            // TODO: panic/throw (more args provided than there were format args)
        }

        template<typename T>
        void format_arg(const T in_arg, uint8_t in_base = 10, bool in_uppercase_digits = false,
                        bool in_prepend_prefix = false);
    };
};

#endif //KERNEL_PLATFORM_X86_64_TYPES_TEXT