#ifndef _STD_LOGGER_HPP
#define _STD_LOGGER_HPP

#include "assert.h"
#include "logger_backend.hpp"
#include "text.hpp"

class logger {
public:
    enum class level : uint8_t { Debug, Info, Warn, Error, Panic };

    void add_backend(logger_backend* in_backend) {
        ASSERT(_num_backends < _MAX_BACKENDS, "too many logger backends");
        _backends[_num_backends++] = in_backend;
    }

    template<typename ... Args>
    void debug(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Debug], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto i = 0; i < _num_backends; i++) {
            _backends[i]->debug(final_msg);
        }
    }

    template<typename ... Args>
    void error(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Error], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto i = 0; i < _num_backends; i++) {
            _backends[i]->error(final_msg);
        }
    }

    void hexdump(const level in_level, const void * in_ptr, size_t in_count,
                 uint8_t in_indent=0);

    template<typename ... Args>
    void info(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Info], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto i = 0; i < _num_backends; i++) {
            _backends[i]->info(final_msg);
        }
    }

    template<typename ... Args>
    void panic(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Panic], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto i = 0; i < _num_backends; i++) {
            _backends[i]->panic(final_msg);
        }
    }

    template<typename ... Args>
    void warn(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Warn], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto i = 0; i < _num_backends; i++) {
            _backends[i]->warn(final_msg);
        }
    }

private:
    static constexpr char _LEVEL_SYMBOLS[] = {
        [(uint8_t)level::Debug] = ' ',
        [(uint8_t)level::Info]  = '+',
        [(uint8_t)level::Warn]  = '-',
        [(uint8_t)level::Error] = '!',
        [(uint8_t)level::Panic] = '*',
    };

    static constexpr char _HEXDUMP_FORMAT[] = "[{}] {}{#016X}:\t";
    static constexpr char _MSG_FORMAT[]     = "[{}] {}\n";
    static constexpr uint8_t _MAX_BACKENDS  = 2;

    logger_backend* _backends[_MAX_BACKENDS] = {0};
    uint8_t _num_backends = 0;
};

#endif // _STD_LOGGER_HPP
