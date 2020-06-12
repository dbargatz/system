#include "logger.hpp"

void logger::debug(text& in_msg) {
    write(level::Debug, in_msg);
}

void logger::error(text& in_msg) {
    write(level::Error, in_msg);
}

void logger::hexdump(const level in_level, const void * in_ptr, size_t in_count) {
    uint8_t * ptr = (uint8_t *)in_ptr;

    for(int i = 0; i < in_count; i++) {
        if(i % 16 == 0) {
            if(i > 0) {
                _serial_port.write("\n");
            }
            _serial_port.write(text(_HEXDUMP_FORMAT,
                _LEVEL_SYMBOLS[(uint8_t)in_level],
                (uint64_t)(ptr+i)).get());
        }
        _serial_port.write(text("{02X} ", *(ptr+i)).get());
    }
    _serial_port.write("\n");
}

void logger::info(text& in_msg) {
    write(level::Info, in_msg);
}

void logger::panic(text& in_msg) {
    write(level::Panic, in_msg);
}

void logger::warn(text& in_msg) {
    write(level::Warning, in_msg);
}

void logger::write(const level in_level, text& in_msg) {
    text final_msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)in_level], in_msg.get());
    _serial_port.write(final_msg);
}
