/**
 * @file
 * @brief Types and functions for working with C-style null-terminated byte
 * strings in C++.
 */
#ifndef _STD_CSTRING_HPP
#define _STD_CSTRING_HPP

#include <cstddef.hpp>
#include <cstdint.hpp>

namespace std {

/**
 * @brief Copies the given number of bytes from the source to destination.
 * 
 * @param in_dest pointer to copy bytes to; must not overlap with in_source
 * @param in_src pointer to copy bytes from; must not overlap with in_destination
 * @param in_count number of bytes to copy
 * @return void* in_destination
 */
extern "C" inline void * memcpy(void * in_dest, const void * in_src, std::size_t in_count) {
    return __builtin_memcpy(in_dest, in_src, in_count);
}

/**
 * @brief Sets the given number of bytes at the given pointer to the given 
 * value.
 * 
 * @param in_dest start of bytes to write to
 * @param in_char value of each byte written
 * @param in_count number of bytes to overwrite
 */
extern "C" inline void memset(void * in_dest, int in_char, std::size_t in_count) {
    __builtin_memset(in_dest, in_char, in_count);
}

/**
 * @brief Returns the number of characters in the given byte string, up to but
 * not including the first null character.
 * 
 * @param in_str pointer to the null-terminated byte string
 * @return length of the null-terminated byte string
 * @warning behavior is undefined if in_str contains no null character
 * @note Space complexity: O(1)
 * @note Time complexity: O(len(in_str))
 */
template <typename T>
std::size_t strlen(const T * in_str) {
    auto ptr = in_str;
    while(((T)'\0') != *ptr) { ptr++; }
    return ptr - in_str -1;
}

}; // namespace std

#endif // _STD_CSTRING_HPP
