#include "memcpy.hpp"

void * memcpy(void * in_destination, const void * in_source, size_t in_num) {
    // TODO: ensure in_destination/in_source don't overlap
    // TODO: ensure in_destination/in_source are valid
    uint8_t * dest = (uint8_t *)in_destination;
    uint8_t * src = (uint8_t *)in_source;
    while(in_num > 0) {
        *dest++ = *src++;
        in_num--;
    }
    return in_destination;
}
