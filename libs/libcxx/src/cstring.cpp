#include <cstring>
#include <cassert>

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
    if(in_count == 0) { return in_dest; }

    auto dest = (std::uint8_t *)in_dest;
    auto src  = (std::uint8_t *)in_src;
    for(auto i = 0; i < in_count; i++) {
        dest[i] = src[i];
    }
    return in_dest;
}

/**
 * @brief Internal implementation. Copies in_count bytes from the object pointed to by in_src to
 * the object pointed to by in_dest. The objects may overlap.
 * 
 * @param in_dest pointer to the memory location to copy to
 * @param in_src pointer to the memory location to copy from
 * @param in_count number of bytes to copy
 * @return in_dest
 */
extern "C" void * _memmove(void * in_dest, const void * in_src, std::size_t in_count) {
    if(in_count == 0 || in_dest == in_src) { return in_dest; }

    auto dest_start = (std::uint8_t *)in_dest;
    auto src_start  = (std::uint8_t *)in_src;
    auto dest_end = dest_start + in_count;
    auto src_end = src_start + in_count;

    // If the objects don't overlap at all, we can simply do a memcpy.
    if(src_end < dest_start || dest_end < src_start) {
        return _memcpy(in_dest, in_src, in_count);
    }

    // The objects overlap, which means we have to be careful how we copy bytes; if we copy in the
    // wrong order, we'll copy already-copied bytes to in_dest instead of the original bytes from
    // in_src. If in_dest starts after in_src, we can copy backwards from src_end to dest_end to
    // avoid incorrect copying; if in_dest starts before in_src, we can simply copy forwards.
    if(dest_start > src_start) {
        assert(src_end > dest_start);
        while(dest_end >= dest_start) {
            *dest_end-- = *src_end--;
        }
    } else {
        assert(dest_start < src_start && dest_end > src_start);
        for(auto i = 0; i < in_count; i++) {
            *dest_start++ = *src_start++;
        }
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
    if(in_count == 0) { return; }

    auto dest = (std::uint8_t *)in_dest;
    for(auto i = 0; i < in_count; i++) {
        dest[i] = (std::uint8_t)in_char;
    }
}

extern "C" void * memcpy(void * in_dest, const void * in_src, std::size_t in_count) {
    return _memcpy(in_dest, in_src, in_count);
}

extern "C" void * memmove(void * in_dest, const void * in_src, std::size_t in_count) {
    return _memmove(in_dest, in_src, in_count);
}

extern "C" void memset(void * in_dest, int in_char, std::size_t in_count) {
    _memset(in_dest, in_char, in_count);
}

void * std::memcpy(void * in_dest, const void * in_src, std::size_t in_count) {
    return _memcpy(in_dest, in_src, in_count);
}

void * std::memmove(void * in_dest, const void * in_src, std::size_t in_count) {
    return _memmove(in_dest, in_src, in_count);
}

void std::memset(void * in_dest, int in_char, std::size_t in_count) {
    _memset(in_dest, in_char, in_count);
}
