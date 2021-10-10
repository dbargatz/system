#ifndef _CORE_CONSOLE_CONSOLE_HPP
#define _CORE_CONSOLE_CONSOLE_HPP

#include <cstddef>
#include <cstdint>
#include <format>
#include <string>

namespace core::console {

/**
 * @brief Relative severity of a message printed to the console.
 */
enum class level : std::uint8_t {
    /**
     * @brief Sentinel value; should not be encountered unless something is uninitialized.
     */
    Unknown,
    /**
     * @brief Highly-detailed messages intended for debugging the core driver; typically filtered
     * out. The term "firehose" comes to mind.
     */
    Debug,
    /**
     * @brief General messages intended for high-level status tracking of the core driver.
     */
    Info,
    /**
     * @brief Messages indicating something has gone wrong in the core driver but it's proceeding
     * despite the issue.
     */
    Warn,
    /**
     * @brief Something terrible has befallen the core driver, and we're likely about to poop the
     * hammock. Usually followed by a panic/abort.
     */
    Error
};

class console {
private:
    constexpr static char8_t _LEVEL_PREFIXES[] = { '?', ' ', '+', '-', '!', '*'};

    template <typename... Args>
    void _write(level in_level, const char8_t* in_fmt, Args&&... in_args) {
        _write(in_level, std::string(in_fmt), in_args...);
    }

    template <typename... Args>
    void _write(level in_level, const char* in_fmt, Args&&... in_args) {
        _write(in_level, std::string((const char8_t*)in_fmt), in_args...);
    }

    template <typename... Args>
    void _write(level in_level, const std::string& in_fmt, Args&&... in_args) {
        auto msg = std::format(in_fmt, in_args...);

        // Don't allow newlines in console messages; this could lead to log forgery when combined
        // with user-manipulated formatting.
        assert(!msg.contains(u8'\n'));

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
