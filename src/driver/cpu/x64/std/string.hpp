#ifndef _STD_STRING_HPP
#define _STD_STRING_HPP

#include "../std/stdint.h"

/**
 * Returns the number of characters in the given string.
 * 
 * @param in_str string to determine length of
 * @return length of string in chars, excluding NULL terminator
 */
extern "C" size_t strlen(const char * in_str);

#endif // _STD_STRING_HPP
