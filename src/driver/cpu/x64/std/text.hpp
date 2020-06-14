#ifndef _STD_TEXT_HPP
#define _STD_TEXT_HPP

#include "stdint.h"


class text {
public:
    template<typename ...Args>
    text(const char * in_format_str, Args&&... in_args) {
        _length_in_chars = 0;
        format(in_format_str, static_cast<Args>(in_args)...);
    }

    const char * get() const { return (const char *)_buf; }

    /**
     * @brief Returns the length in characters.
     * 
     * @note does not include null terminator.
     * @note disregards encoding; multi-byte character sequences count as a
     *       single character.
     * @return length of the text in characters
     */
    const size_t length() const { return _length_in_chars; }

private:
    char _buf[512];
    size_t _length_in_chars;

    bool _is_digit(char in_char, bool in_exclude_zero = false) {
        char min_char = in_exclude_zero ? '1' : '0';
        return (min_char <= in_char && '9' >= in_char);
    }

    // TODO: rename with underscore
    void format(const char * in_format_str);

    // TODO: rename with underscore
    template<typename T, typename ...RemainingArgs>
    void format(const char * in_format_str, const T& in_arg, RemainingArgs&&... in_remaining_args) {
        // TODO: Comment on behavior/implementation; strives to conform with
        //       format specification mini-language from Python 3:
        //       https://docs.python.org/3/library/string.html#format-specification-mini-language 
        while('\0' != *in_format_str) {
            if('{' == *in_format_str) {
                ++in_format_str;
                
                if('\0' == *in_format_str) {
                    // TODO: panic/throw (single open brace at end of string)
                    return;
                }
                else if('{' != *in_format_str) {
                    char fill = ' ';
                    bool prepend_prefix = false;
                    uint64_t min_width = 0;

                    // TODO: handle positional index (must be followed by 
                    //       colon if any of the below fields are specified)

                    // TODO: handle padding char
                    // TODO: handle padding alignment
                    // TODO: handle sign
                    
                    // Handle the "alternate form" option, if present.
                    if('#' == *in_format_str) {
                        prepend_prefix = true;
                        ++in_format_str;
                    }

                    // TODO: handle padding width
                    if(_is_digit(*in_format_str)) {
                        // TODO: if a fill character and alignment character 
                        //       were specified, should they override a 
                        //       provided '0' prefixing the width field?

                        // Handle the optional preceding '0', which is 
                        // equivalent to setting alignment to '=' (padding
                        // after the sign but before the digits) and setting
                        // the fill char to '0'.
                        if('0' == *in_format_str) {
                            fill = '0';
                            ++in_format_str;

                            // Ensure the next field is a digit between '1' 
                            // and '9'; the '0' prefix is invalid if it
                            // doesn't precede a valid width field.
                            if(!_is_digit(*in_format_str, true)) {
                                // TODO: panic/throw (can't have '0' prefix
                                //       without valid width)
                                return;
                            }
                        }

                        // Handle the minimum width field, which must be a 
                        // valid decimal integer greater than 0.
                        while(_is_digit(*in_format_str, true)) {
                            min_width *= 10;
                            min_width += *in_format_str - '0';
                            ++in_format_str;
                        }
                    }

                    // TODO: handle argument format (x, X, d, s, etc)
                    if('d' == *in_format_str) {
                        format_arg(in_arg, 10, false, prepend_prefix, 
                                    min_width, fill);
                        ++in_format_str;
                    }
                    else if('s' == *in_format_str) {
                        format_arg(in_arg, 0, false, prepend_prefix, 
                                    min_width, fill);
                        ++in_format_str;
                    }
                    else if('x' == *in_format_str) {
                        format_arg(in_arg, 16, false, prepend_prefix, 
                                    min_width, fill);
                        ++in_format_str;
                    }
                    else if('X' == *in_format_str) {
                        format_arg(in_arg, 16, true, prepend_prefix, 
                                    min_width, fill);
                        ++in_format_str;
                    }
                    else if('f' == *in_format_str) {
                        format_arg(in_arg, 10, false, prepend_prefix, min_width,
                            fill);
                        ++in_format_str;
                    }
                    else {
                        // Format argument using defaults. Do not advance.
                        format_arg(in_arg, 10, false, prepend_prefix,
                                    min_width, fill);
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

    // TODO: rename with underscore
    template<typename T>
    void format_arg(const T in_arg, 
                    uint8_t in_base, 
                    bool in_uppercase_digits,
                    bool in_prepend_prefix,
                    uint64_t in_min_width, 
                    char in_fill_char);
};

#endif // _STD_TEXT_HPP
