#include "cstring.hpp"

std::size_t strlen(const char * in_str) {
    const char * end = in_str;
    while('\0' != *in_str++);
    return end - in_str -1;
}
