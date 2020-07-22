#include "string.hpp"

size_t strlen(const char * in_str) {
    auto sz = 0;
    while('\0' != *in_str++) {
        sz++;
    }
    return sz;
}
