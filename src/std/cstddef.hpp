/**
 * @file
 * @brief Contains definitions of commonly-used types such as size_t, byte, and
 * nullptr_t.
 */
#ifndef _STD_CSTDDEF_HPP
#define _STD_CSTDDEF_HPP

#include <cstdint.hpp>

namespace std {
    using max_align_t = long double;
    using nullptr_t   = decltype(nullptr);
    using ptrdiff_t   = int64_t;
    using size_t      = uint64_t;
    // TODO: byte
}; //namespace std

#define offsetof(st, m) __builtin_offsetof(st, m);
#define NULL ((void *)0)

#endif // _STD_CSTDDEF_HPP
