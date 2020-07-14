#ifndef _STD_LOGGER_HPP
#define _STD_LOGGER_HPP

#include "array.hpp"
#include "logger_backend.hpp"
#include "text.hpp"

class logger {
public:
    enum class level : uint8_t { Debug, Info, Warn, Error, Panic };

    logger(std::array<logger_backend*,2> in_backends) : _backends{in_backends[0], in_backends[1]} {
        auto be1 = in_backends[0];
        auto be2 = in_backends[1];
        be1->debug(text("BE0: {X}", (uint64_t)be1).get());
        be2->debug(text("BE1: {X}", (uint64_t)be2).get());
    }

    template<typename ... Args>
    void debug(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Debug], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto be : _backends) {
            be->debug(final_msg);
        }
    }

    template<typename ... Args>
    void error(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Error], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto be : _backends) {
            be->error(final_msg);
        }
    }

    void hexdump(const level in_level, const void * in_ptr, size_t in_count,
                 uint8_t in_indent=0);

    template<typename ... Args>
    void info(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Info], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto be : _backends) {
            be->info(final_msg);
        }
    }

    template<typename ... Args>
    void panic(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Panic], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto be : _backends) {
            be->panic(final_msg);
        }
    }

    template<typename ... Args>
    void warn(const text& in_fmt, Args&&... in_args) {
        text msg(_MSG_FORMAT, _LEVEL_SYMBOLS[(uint8_t)level::Warn], in_fmt.get());
        text final_msg(msg.get(), static_cast<Args>(in_args)...);
        for(auto be : _backends) {
            be->warn(final_msg);
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
    static constexpr char _MSG_FORMAT[] = "[{}] {}\n";

    logger_backend* _backends[2];
};

#endif // _STD_LOGGER_HPP
