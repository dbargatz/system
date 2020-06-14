#include "text.hpp"

void text::format(const char * in_format_str) {
    while('\0' != *in_format_str) {
        if('{' == *in_format_str) {
            // TODO: panic/throw (there were more args in the format string than args provided)
            return;
        }

        // TODO: bounds-check _buf
        _buf[_length_in_chars++] = *in_format_str++;
    }

    // TODO: bounds-check _buf
    _buf[_length_in_chars] = '\0';
}

template<> void text::format_arg(const char in_arg,
                                 uint8_t in_base,
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width,
                                 char in_fill_char) {
    // TODO: how to handle multi-byte chars?
    _buf[_length_in_chars++] = in_arg;
}

template<> void text::format_arg(const char * in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    // TODO: bounds-check _buf
    while('\0' != *in_arg) {
        _buf[_length_in_chars++] = *in_arg++;
    }
}

template<> void text::format_arg(const float64_t in_arg,
                                 uint8_t in_base,
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width,
                                 char in_fill_char) {
    char temp[64] = {0};
    size_t idx = 0;
    const char * digits = "0123456789";
    int64_t whole = (int64_t)in_arg;
    // TODO: make precision an argument
    uint64_t fractional = ((in_arg - whole) * 1000000.0); // Move fractional component up by 10^6 places for precision of 6

    // TODO: verify this algorithm is actually correct?!
    // Do the fractional part.
    if(fractional == 0) {
        temp[idx++] = digits[0];
    }
    else {
        while(fractional != 0) {
            temp[idx++] = digits[fractional % in_base];
            fractional /= in_base;
        }
    }

    // Put the decimal point in.
    temp[idx++] = '.';

    // Now do the whole number part.
    if(whole == 0) {
        temp[idx++] = digits[0];
    }
    else {
        while(whole != 0) {
            temp[idx++] = digits[whole % in_base];
            whole /= in_base;
        }
    }

    // If the number of digits is less than the minimum field width, append the
    // fill character until reaching the minimum field width. Note that this 
    // effectively right-aligns the number, as the temp buffer is in reverse
    // order.
    // TODO: bounds-check temp, or dynamically resize it; otherwise, buffer 
    //       overflow with crafted minimum field width
    while(idx < in_min_width) {
        temp[idx++] = in_fill_char;
    }

    if(in_arg < 0) {
        _buf[_length_in_chars++] = '-';
    }

    // TODO: bounds-check _buf
    while(idx != 0) {
        _buf[_length_in_chars++] = temp[--idx];
    }
}

template<> void text::format_arg(const int64_t in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    char temp[32] = {0};
    size_t idx = 0;
    const char * digits = in_uppercase_digits ? "0123456789ABCDEF" : "0123456789abcdef";
    uint64_t remainder = in_arg;

    if(0 == remainder) {
        temp[idx++] = digits[0];
    }
    else {
        while(remainder != 0) {
            temp[idx++] = digits[remainder % in_base];
            remainder /= in_base;
        }
    }

    // If the number of digits is less than the minimum field width, append the
    // fill character until reaching the minimum field width. Note that this 
    // effectively right-aligns the number, as the temp buffer is in reverse
    // order.
    // TODO: bounds-check temp, or dynamically resize it; otherwise, buffer 
    //       overflow with crafted minimum field width
    while(idx < in_min_width) {
        temp[idx++] = in_fill_char;
    }

    if(in_arg < 0) {
        _buf[_length_in_chars++] = '-';
    }

    if(in_prepend_prefix && 2 == in_base) {
        _buf[_length_in_chars++] = '0';
        _buf[_length_in_chars++] = 'b';
    }
    else if(in_prepend_prefix && 16 == in_base) {
        _buf[_length_in_chars++] = '0';
        _buf[_length_in_chars++] = 'x';
    }

    // TODO: bounds-check _buf
    while(idx != 0) {
        _buf[_length_in_chars++] = temp[--idx];
    }
}

template<> void text::format_arg(const uint64_t in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    char temp[32] = {0};
    size_t idx = 0;
    const char * digits = in_uppercase_digits ? "0123456789ABCDEF" : "0123456789abcdef";
    uint64_t remainder = in_arg;

    if(0 == remainder) {
        temp[idx++] = digits[0];
    }
    else {
        while(remainder != 0) {
            temp[idx++] = digits[remainder % in_base];
            remainder /= in_base;
        }
    }

    // If the number of digits is less than the minimum field width, append the
    // fill character until reaching the minimum field width. Note that this 
    // effectively right-aligns the number, as the temp buffer is in reverse
    // order.
    // TODO: bounds-check temp, or dynamically resize it; otherwise, buffer 
    //       overflow with crafted minimum field width
    while(idx < in_min_width) {
        temp[idx++] = in_fill_char;
    }

    if(in_prepend_prefix && 2 == in_base) {
        _buf[_length_in_chars++] = '0';
        _buf[_length_in_chars++] = 'b';
    }
    else if(in_prepend_prefix && 16 == in_base) {
        _buf[_length_in_chars++] = '0';
        _buf[_length_in_chars++] = 'x';
    }

    // TODO: bounds-check _buf
    while(idx != 0) {
        _buf[_length_in_chars++] = temp[--idx];
    }
}

template<> void text::format_arg(const uint8_t in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((uint64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const uint16_t in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((uint64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const uint32_t in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((uint64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const int8_t in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((int64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const int16_t in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((int64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const int32_t in_arg, 
                                 uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((int64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const float32_t in_arg,
                                 uint8_t in_base,
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 uint64_t in_min_width,
                                 char in_fill_char) {
    format_arg((float64_t)in_arg, in_base, in_uppercase_digits,
               in_prepend_prefix, in_min_width, in_fill_char);
}
