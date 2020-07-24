/**
 * @file
 * @brief Contains definitions of commonly-used types such as size_t, byte, and
 * nullptr_t.
 */
#ifndef _STD_CSTDDEF_HPP
#define _STD_CSTDDEF_HPP

namespace std {
    using nullptr_t = decltype(nullptr);
    using size_t = unsigned long long;
    // TODO: ptrdiff_t
    // TODO: byte
    // TODO: max_align_t
    // TODO: offsetof()
}; //namespace std

#endif // _STD_CSTDDEF_HPP
