#ifndef _LOGGING_LOGGER_HPP
#define _LOGGING_LOGGER_HPP

#include <cstdint.hpp>
#include <format.hpp>
#include <string.hpp>
#include <utility.hpp>

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

    template <typename... Args>
    void debug(const char8_t * in_fmt, Args&&... in_args) {
        std::format(std::string(in_fmt), std::forward<Args>(in_args)...);
    }
};

}; // namespace logging

#endif // _LOGGING_LOGGER_HPP