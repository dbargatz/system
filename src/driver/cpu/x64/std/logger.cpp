#include "logger.hpp"

void logger::hexdump(const level in_level, const void * in_ptr, size_t in_count,
                     uint8_t in_indent) {
    char indents[] = "\t\t\t\t\t\t";
    uint8_t * ptr = (uint8_t *)in_ptr;

    // TODO: assert in_indent <= strlen(indents)
    indents[in_indent] = '\0';

    for(int i = 0; i < in_count; i++) {
        if(i % 16 == 0) {
            if(i > 0) {
                _backends.panic("\n");
            }
            _backends.panic(text(_HEXDUMP_FORMAT,
                _LEVEL_SYMBOLS[(uint8_t)in_level],
                indents,
                (uint64_t)(ptr+i)).get());
        }
        _backends.panic(text("{02X} ", *(ptr+i)).get());
    }
    _backends.panic("\n");
}
