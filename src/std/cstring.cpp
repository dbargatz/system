#include <cstring>

/**
 * @brief Internal implementation. Copies the given number of bytes from the
 * source to destination.
 * 
 * @param in_dest pointer to copy bytes to; must not overlap with in_src
 * @param in_src pointer to copy bytes from; must not overlap with in_dest
 * @param in_count number of bytes to copy
 * @return void* in_dest
 */
extern "C" void * _memcpy(void * in_dest, const void * in_src, std::size_t in_count) {
    auto dest = (std::uint8_t *)in_dest;
    auto src  = (std::uint8_t *)in_src;
    for(auto i = 0; i < in_count; i++) {
        dest[i] = src[i];
    }
    return in_dest;
}

/**
 * @brief Internal implementation. Sets the given number of bytes at the given
 * pointer to the given value.
 * 
 * @param in_dest start of bytes to write to
 * @param in_char value of each byte written
 * @param in_count number of bytes to overwrite
 */
extern "C" void _memset(void * in_dest, int in_char, std::size_t in_count) {
    auto dest = (std::uint8_t *)in_dest;
    for(auto i = 0; i < in_count; i++) {
        dest[i] = (std::uint8_t)in_char;
    }
}

/**
 * @brief Copies the given number of bytes from the source to destination.
 * 
 * @param in_dest pointer to copy bytes to; must not overlap with in_src
 * @param in_src pointer to copy bytes from; must not overlap with in_dest
 * @param in_count number of bytes to copy
 * @return void* in_dest
 */
extern "C" void * memcpy(void * in_dest, const void * in_src, std::size_t in_count) {
    return _memcpy(in_dest, in_src, in_count);
}

/**
 * @brief Sets the given number of bytes at the given pointer to the given 
 * value.
 * 
 * @param in_dest start of bytes to write to
 * @param in_char value of each byte written
 * @param in_count number of bytes to overwrite
 */
extern "C" void memset(void * in_dest, int in_char, std::size_t in_count) {
    _memset(in_dest, in_char, in_count);
}

void * std::memcpy(void * in_dest, const void * in_src, std::size_t in_count) {
    return _memcpy(in_dest, in_src, in_count);
}

void std::memset(void * in_dest, int in_char, std::size_t in_count) {
    _memset(in_dest, in_char, in_count);
}
