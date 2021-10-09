#ifndef _CORE_CONSOLE_CONSOLE_HPP
#define _CORE_CONSOLE_CONSOLE_HPP

#include <cstddef>
#include <cstdint>
#include <format>
#include <string>

namespace core::console {

enum class level : std::uint8_t {
    Unknown,
    Debug,
    Info,
    Warn,
    Error
};

class console {
private:
    constexpr static char8_t _LEVEL_PREFIXES[] = { '?', ' ', '+', '-', '!', '*'};

    // TODO: This is dangerous for a few reasons, security-wise:
    //       + We don't filter out newlines or other spacing
    //       + We don't disallow prefixes used as log level prefixes
    //       + A bunch of other nefarious stuff can be done - we don't filter at all
    // Someone can forge log messages as a result of these shortfalls. They need
    // to be corrected.
    template <typename... Args>
    void _write(level in_level, const char8_t* in_fmt, Args&&... in_args) { _write(in_level, std::string(in_fmt), in_args...); }
    template <typename... Args>
    void _write(level in_level, const char* in_fmt, Args&&... in_args) { _write(in_level, std::string((const char8_t*)in_fmt), in_args...); }
    template <typename... Args>
    void _write(level in_level, const std::string& in_fmt, Args&&... in_args) {
        auto msg = std::format(in_fmt, in_args...);
        auto lvl = _LEVEL_PREFIXES[(std::uint8_t)in_level];
        auto final = std::format(u8"[{}] {}\n", lvl, msg);
        _platform_write(final.c_str());
    }

    void _platform_write(const char8_t* in_str);

public:
    console();

    template <typename... Args>
    void debug(const char8_t* in_fmt, Args&&... in_args) { _write(level::Debug, in_fmt, in_args...); }
    template <typename... Args>
    void debug(const char* in_fmt, Args&&... in_args) { _write(level::Debug, in_fmt, in_args...); }
    template <typename... Args>
    void debug(const std::string& in_fmt, Args&&... in_args) { _write(level::Debug, in_fmt, in_args...); }

    template <typename... Args>
    void info(const char8_t* in_fmt, Args&&... in_args) { _write(level::Info, in_fmt, in_args...); }
    template <typename... Args>
    void info(const char* in_fmt, Args&&... in_args) { _write(level::Info, in_fmt, in_args...); }
    template <typename... Args>
    void info(const std::string& in_fmt, Args&&... in_args) { _write(level::Info, in_fmt, in_args...); }

    template <typename... Args>
    void warn(const char8_t* in_fmt, Args&&... in_args) { _write(level::Warn, in_fmt, in_args...); }
    template <typename... Args>
    void warn(const char* in_fmt, Args&&... in_args) { _write(level::Warn, in_fmt, in_args...); }
    template <typename... Args>
    void warn(const std::string& in_fmt, Args&&... in_args) { _write(level::Warn, in_fmt, in_args...); }

    template <typename... Args>
    void error(const char8_t* in_fmt, Args&&... in_args) { _write(level::Error, in_fmt, in_args...); }
    template <typename... Args>
    void error(const char* in_fmt, Args&&... in_args) { _write(level::Error, in_fmt, in_args...); }
    template <typename... Args>
    void error(const std::string& in_fmt, Args&&... in_args) { _write(level::Error, in_fmt, in_args...); }

    template <typename... Args>
    void log(level in_level, const char8_t* in_fmt, Args&&... in_args) { _write(in_level, in_fmt, in_args...); }
    template <typename... Args>
    void log(level in_level, const char* in_fmt, Args&&... in_args) { _write(in_level, in_fmt, in_args...); }
    template <typename... Args>
    void log(level in_level, const std::string& in_fmt, Args&&... in_args) { _write(in_level, in_fmt, in_args...); }
};

};

#endif // _CORE_CONSOLE_CONSOLE_HPP
