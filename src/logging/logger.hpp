#ifndef _LOGGING_LOGGER_HPP
#define _LOGGING_LOGGER_HPP

#include <cassert.hpp>
#include <cstdint.hpp>
#include <format.hpp>
#include <string.hpp>
#include <utility.hpp>

#include "backend.hpp"

namespace logging {

enum class level : std::uint8_t {
    Unknown,
    Debug,
    Info,
    Warn,
    Error,
    Panic
};

class logger {
public:
    // TODO: ability to name loggers without losing DI?
    logger() = default;

    void add_backend(backend* in_backend) {
        assert(_num_backends < _MAX_BACKENDS);
        _backends[_num_backends++] = in_backend;
    }

    template <typename... Args>
    void debug(const std::string& in_fmt, Args&&... in_args) {
        write(level::Debug, in_fmt, std::forward<Args>(in_args)...);
    }

    template <typename... Args>
    void error(const std::string& in_fmt, Args&&... in_args) {
        write(level::Error, in_fmt, std::forward<Args>(in_args)...);
    }

    template <typename... Args>
    void info(const std::string& in_fmt, Args&&... in_args) {
        write(level::Info, in_fmt, std::forward<Args>(in_args)...);
    }

    template <typename... Args>
    void panic(const std::string& in_fmt, Args&&... in_args) {
        write(level::Panic, in_fmt, std::forward<Args>(in_args)...);
    }

    template <typename... Args>
    void warn(const std::string& in_fmt, Args&&... in_args) {
        write(level::Warn, in_fmt, std::forward<Args>(in_args)...);
    }

    // TODO: This is dangerous for a few reasons, security-wise:
    //       + We don't filter out newlines or other spacing
    //       + We don't disallow prefixes used as log level prefixes
    //       + A bunch of other nefarious stuff can be done - we don't filter at all
    // Someone can forge log messages as a result of these shortfalls. They need
    // to be corrected.
    template <typename... Args>
    void write(level in_level, const std::string& in_fmt, Args&&... in_args) {
        auto str = std::format(in_fmt, std::forward<Args>(in_args)...);
        auto final = std::format(u8"[{}] {}\n", _LEVEL_PREFIXES[(std::uint8_t)in_level], str);
        for(auto i = 0; i < _num_backends; i++) {
            _backends[i]->write(final);
        }
    }

private:
    constexpr static char8_t _LEVEL_PREFIXES[] = {
        [(std::uint8_t)level::Unknown] = '?',
        [(std::uint8_t)level::Debug]   = ' ',
        [(std::uint8_t)level::Info]    = '+',
        [(std::uint8_t)level::Warn]    = '-',
        [(std::uint8_t)level::Error]   = '!',
        [(std::uint8_t)level::Panic]   = '*',
    };

    constexpr static std::uint8_t _MAX_BACKENDS = 2;
    backend* _backends[_MAX_BACKENDS] = {0};
    std::uint8_t _num_backends = 0;
};

}; // namespace logging

#endif // _LOGGING_LOGGER_HPP
