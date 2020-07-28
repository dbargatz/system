#include "text.hpp"
#include <cassert.hpp>
#include <cstring.hpp>

text::text(const char * in_str) : _length_in_chars(0) {
    format(in_str);
}

text::text(const text& in_other) : _length_in_chars(0) {
    auto len = in_other.length();
    assert(len < (_MAX_LENGTH_BYTES - 1));
    std::memcpy(_buf, in_other._buf, len);
    _buf[len] = '\0';
    _length_in_chars = in_other.length();
}

void text::format(const char * in_format_str) {
    while('\0' != *in_format_str) {
        assert(_length_in_chars < (_MAX_LENGTH_BYTES - 1));
        _buf[_length_in_chars++] = *in_format_str++;
    }

    _buf[_length_in_chars] = '\0';
}

template<> void text::format_arg(const char in_arg,
                                 std::uint8_t in_base,
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width,
                                 char in_fill_char) {
    // TODO: how to handle multi-byte chars?
    _buf[_length_in_chars++] = in_arg;
}

template<> void text::format_arg(const char * in_arg,
                                 std::uint8_t in_base,
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width,
                                 char in_fill_char) {
    while('\0' != *in_arg) {
        assert(_length_in_chars < (_MAX_LENGTH_BYTES - 1));
        _buf[_length_in_chars++] = *in_arg++;
    }
}

template<> void text::format_arg(const text in_arg,
                                 std::uint8_t in_base,
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width,
                                 char in_fill_char) {

    auto len = in_arg.length();
    assert((_length_in_chars + len) < (_MAX_LENGTH_BYTES - 1));
    std::memcpy(&_buf[_length_in_chars], in_arg._buf, len);
    _length_in_chars += len;
}

template<> void text::format_arg(const double in_arg,
                                 std::uint8_t in_base,
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width,
                                 char in_fill_char) {
    char temp[64] = {0};
    std::size_t idx = 0;
    const char * digits = "0123456789";
    std::int64_t whole = (std::int64_t)in_arg;
    // TODO: make precision an argument
    // Move fractional component up by 10^6 places for precision of 6
    std::uint64_t fractional = ((in_arg - whole) * 1000000.0);

    // TODO: verify this algorithm is actually correct?!
    // Do the fractional part.
    if(fractional == 0) {
        assert(idx < (sizeof(temp) - 1));
        temp[idx++] = digits[0];
    }
    else {
        while(fractional != 0) {
            assert(idx < (sizeof(temp) - 1));
            temp[idx++] = digits[fractional % in_base];
            fractional /= in_base;
        }
    }

    // Put the decimal point in.
    assert(idx < (sizeof(temp) - 1));
    temp[idx++] = '.';

    // Now do the whole number part.
    if(whole == 0) {
        assert(idx < (sizeof(temp) - 1));
        temp[idx++] = digits[0];
    }
    else {
        while(whole != 0) {
            assert(idx < (sizeof(temp) - 1));
            temp[idx++] = digits[whole % in_base];
            whole /= in_base;
        }
    }

    // If the number of digits is less than the minimum field width, append the
    // fill character until reaching the minimum field width. Note that this 
    // effectively right-aligns the number, as the temp buffer is in reverse
    // order.
    while(idx < in_min_width) {
        assert(idx < (sizeof(temp) - 1));
        temp[idx++] = in_fill_char;
    }

    if(in_arg < 0) {
        assert(_length_in_chars < (sizeof(_buf) - 1));
        _buf[_length_in_chars++] = '-';
    }

    while(idx != 0) {
        assert(_length_in_chars < (sizeof(_buf) - 1));
        _buf[_length_in_chars++] = temp[--idx];
    }
}

template<> void text::format_arg(const std::int64_t in_arg, 
                                 std::uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width, 
                                 char in_fill_char) {
    char temp[32] = {0};
    std::size_t idx = 0;
    const char * digits = in_uppercase_digits ? "0123456789ABCDEF" : "0123456789abcdef";
    std::int64_t remainder = in_arg;
    std::int8_t modulo;

    if(0 == remainder) {
        assert(idx < (sizeof(temp) - 1));
        temp[idx++] = digits[0];
    }
    else {
        // TODO: This is super hacky - works for now, but this whole class is
        //       just formatting hacks on top of hacks.
        while(remainder != 0) {
            modulo = remainder % in_base;
            if(modulo < 0) {
                modulo *= -1;
            }
            assert(idx < (sizeof(temp) - 1));
            temp[idx++] = digits[modulo];
            remainder /= in_base;
        }
    }

    // If the number of digits is less than the minimum field width, append the
    // fill character until reaching the minimum field width. Note that this
    // effectively right-aligns the number, as the temp buffer is in reverse
    // order.
    while(idx < in_min_width) {
        assert(idx < (sizeof(temp) - 1));
        temp[idx++] = in_fill_char;
    }

    if(in_arg < 0) {
        assert(_length_in_chars < (sizeof(_buf) - 1));
        _buf[_length_in_chars++] = '-';
    }

    if(in_prepend_prefix && 2 == in_base) {
        assert(_length_in_chars < (sizeof(_buf) - 2));
        _buf[_length_in_chars++] = '0';
        _buf[_length_in_chars++] = 'b';
    }
    else if(in_prepend_prefix && 16 == in_base) {
        assert(_length_in_chars < (sizeof(_buf) - 2));
        _buf[_length_in_chars++] = '0';
        _buf[_length_in_chars++] = 'x';
    }

    while(idx != 0) {
        assert(_length_in_chars < (sizeof(_buf) - 1));
        _buf[_length_in_chars++] = temp[--idx];
    }
}

template<> void text::format_arg(const std::uint64_t in_arg, 
                                 std::uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width, 
                                 char in_fill_char) {
    char temp[32] = {0};
    std::size_t idx = 0;
    const char * digits = in_uppercase_digits ? "0123456789ABCDEF" : "0123456789abcdef";
    std::uint64_t remainder = in_arg;

    if(0 == remainder) {
        assert(idx < (sizeof(temp) - 1));
        temp[idx++] = digits[0];
    }
    else {
        while(remainder != 0) {
            assert(idx < (sizeof(temp) - 1));
            temp[idx++] = digits[remainder % in_base];
            remainder /= in_base;
        }
    }

    // If the number of digits is less than the minimum field width, append the
    // fill character until reaching the minimum field width. Note that this 
    // effectively right-aligns the number, as the temp buffer is in reverse
    // order.
    while(idx < in_min_width) {
        assert(idx < (sizeof(temp) - 1));
        temp[idx++] = in_fill_char;
    }

    if(in_prepend_prefix && 2 == in_base) {
        assert(_length_in_chars < (sizeof(_buf) - 2));
        _buf[_length_in_chars++] = '0';
        _buf[_length_in_chars++] = 'b';
    }
    else if(in_prepend_prefix && 16 == in_base) {
        assert(_length_in_chars < (sizeof(_buf) - 2));
        _buf[_length_in_chars++] = '0';
        _buf[_length_in_chars++] = 'x';
    }

    while(idx != 0) {
        assert(_length_in_chars < (sizeof(_buf) - 1));
        _buf[_length_in_chars++] = temp[--idx];
    }
}

template<> void text::format_arg(const std::uint8_t in_arg, 
                                 std::uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((std::uint64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const std::uint16_t in_arg, 
                                 std::uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((std::uint64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const std::uint32_t in_arg, 
                                 std::uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((std::uint64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const std::int8_t in_arg, 
                                 std::uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((std::int64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const std::int16_t in_arg, 
                                 std::uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((std::int64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const std::int32_t in_arg, 
                                 std::uint8_t in_base, 
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width, 
                                 char in_fill_char) {
    format_arg((std::int64_t)in_arg, in_base, in_uppercase_digits, 
               in_prepend_prefix, in_min_width, in_fill_char);
}

template<> void text::format_arg(const float in_arg,
                                 std::uint8_t in_base,
                                 bool in_uppercase_digits,
                                 bool in_prepend_prefix,
                                 std::uint64_t in_min_width,
                                 char in_fill_char) {
    format_arg((double)in_arg, in_base, in_uppercase_digits,
               in_prepend_prefix, in_min_width, in_fill_char);
}
