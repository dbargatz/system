/**
 * @file
 * @brief Types and functions for working with C-style null-terminated byte
 * strings in C++.
 */
#ifndef _STD_CSTRING_HPP
#define _STD_CSTRING_HPP

// TODO: how to angle-bracket include cstddef (specify self as stdlib?)
#include "cstddef.hpp"

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
std::size_t strlen(const char * in_str);

#endif // _STD_CSTRING_HPP
