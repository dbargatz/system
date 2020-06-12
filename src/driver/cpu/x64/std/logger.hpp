#ifndef _STD_LOGGER_HPP
#define _STD_LOGGER_HPP

#include "text.hpp"
#include "../debug/serial.hpp"

class logger {
public:
    enum class level { Debug, Info, Warning, Error, Panic };

    logger(SerialPort& in_serial_port) : _serial_port(in_serial_port) {}

    void debug(text& in_msg);
    template<typename ... Args>
    void debug(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        debug(msg);
    }

    void error(text& in_msg);
    template<typename ... Args>
    void error(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        error(msg);
    }

    void hexdump(const level in_level, const void * in_ptr, size_t in_count);

    void info(text& in_msg);
    template<typename ... Args>
    void info(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        info(msg);
    }

    void panic(text& in_msg);
    template<typename ... Args>
    void panic(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        panic(msg);
    }

    void warn(text& in_msg);
    template<typename ... Args>
    void warn(const char * in_format_str, Args&&... in_args) {
        text msg(in_format_str, static_cast<Args>(in_args)...);
        warn(msg);
    }

    void write(const level in_level, text& in_msg);

private:
    static constexpr char _LEVEL_SYMBOLS[] = { 
        [(uint8_t)level::Debug]   = ' ',
        [(uint8_t)level::Info]    = '!',
        [(uint8_t)level::Warning] = '+',
        [(uint8_t)level::Error]   = '-',
        [(uint8_t)level::Panic]   = '*',
    };

    static constexpr char _HEXDUMP_FORMAT[] = "[{}] {#016X}:\t";
    static constexpr char _MSG_FORMAT[] = "[{}] {}";

    SerialPort& _serial_port;
};

#endif // _STD_LOGGER_HPP
