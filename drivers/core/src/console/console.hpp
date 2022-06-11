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
    level _current_level = level::Info;
    constexpr static char _LEVEL_PREFIXES[] = { '?', ' ', '+', '-', '!', '*'};

    void _write(level in_level, const char * in_str) const {
        // Don't format or log the message if it's below the current log level.
        if(in_level < _current_level) { return; }

        _platform_write(in_level);
        _platform_write(in_str);
        _platform_write("\n");
    }

    template <typename... Args>
    void _write(level in_level, const char8_t* in_fmt, Args&&... in_args) const {
        _write(in_level, std::string((const char *)in_fmt), in_args...);
    }

    template <typename... Args>
    void _write(level in_level, const char* in_fmt, Args&&... in_args) const {
        _write(in_level, std::string(in_fmt), in_args...);
    }

    template <typename... Args>
    void _write(level in_level, const std::string& in_fmt, Args&&... in_args) const {
        // Don't format or log the message if it's below the current log level.
        if(in_level < _current_level) { return; }

        std::string msg = std::format(in_fmt, in_args...);
        auto lvl = _LEVEL_PREFIXES[(std::uint8_t)in_level];
        auto start_idx = 0;
        auto count = 0;
        while(start_idx < msg.length()) {
            auto newline_idx = msg.find('\n', start_idx);
            if(newline_idx == std::string::npos) {
                count = msg.length() - start_idx;
            } else {
                count = newline_idx - start_idx;
            }

            auto line = std::string(msg, start_idx, count);
            auto final = std::format("[{}] {}\n", lvl, line);
            _platform_write(final.c_str());
            start_idx += count + 1;
        }
    }

    bool _platform_init(void) const;
    void _platform_write(const char in_c) const;

    void _platform_write(level in_level) const {
        auto lvl = _LEVEL_PREFIXES[(std::uint8_t)in_level];
        _platform_write('[');
        _platform_write(lvl);
        _platform_write("] ");
    }

    void _platform_write(const char * in_str) const {
        for (int i = 0; in_str[i] != '\0'; i++) {
            // Logging only uses newlines (\n), but TTY serial connections expect a
            // carriage return (\r) followed by a newline (\n). Without the \r, the
            // cursor never returns to the start of a line, so messages with just a
            // newline end up looking like this:
            // This is line 1
            //               This is line 2
            //                             See why the \r is needed?
            // To avoid this, put a carriage return before putting any newlines.
            // It's okay if the newline already had a carriage return before it;
            // sending multiple carriage returns does nothing other than waste a
            // bit of time.
            if (in_str[i] == '\n') {
                _platform_write('\r');
            }

            // Send this character.
            _platform_write(in_str[i]);
        }
    }

public:
    console(level in_level = level::Info) {
        assert(_platform_init());
        set_level(in_level);
        debug("Console init");
    }

    /**
     * @brief Returns the current console message level.
     * 
     * @return current console message level 
     */
    level get_level(void) {
        return _current_level;
    }

    /**
     * @brief Sets the current console level to in_level, meaning messages with a level below this
     * will not be written to the console.
     * 
     * @param in_level minimum level of all messages to write to the console; cannot be Unknown
     */
    void set_level(level in_level) {
        assert(in_level != level::Unknown);
        _current_level = in_level;
    }

    /**
     * @brief Writes a series of messages in various Unicode languages to the console at in_level.
     * Intended to exercise Unicode functionality on new platforms.
     * 
     * @param in_level level to write the messages at; if level is below the current console level,
     * no messages will be written to the console
     */
    void unicode_test(level in_level) {
        if(in_level < _current_level) { return; }

        log(in_level, "Console Unicode test:");
        log(in_level, "  Arabic:   هذا اختبار");
        log(in_level, "  Chinese:  这是一个测试");
        log(in_level, "  Emojis:   🙃 ❤️‍🔥 🖕");
        log(in_level, "  English:  this is a test");
        log(in_level, "  Hindi:    यह एक परीक्षण है");
        log(in_level, "  Japanese: これはテストです");
        log(in_level, "  Korean:   이것은 시험이다");
        log(in_level, "  Russian:  это проверка");
    }

    void debug(const char * in_str) const { _write(level::Debug, in_str); }
    template <typename... Args>
    void debug(const char8_t* in_fmt, Args&&... in_args) const { _write(level::Debug, in_fmt, in_args...); }
    template <typename... Args>
    void debug(const char* in_fmt, Args&&... in_args) const { _write(level::Debug, in_fmt, in_args...); }
    template <typename... Args>
    void debug(const std::string& in_fmt, Args&&... in_args) const { _write(level::Debug, in_fmt, in_args...); }

    void info(const char * in_str) const { _write(level::Info, in_str); }
    template <typename... Args>
    void info(const char8_t* in_fmt, Args&&... in_args) const { _write(level::Info, in_fmt, in_args...); }
    template <typename... Args>
    void info(const char* in_fmt, Args&&... in_args) const { _write(level::Info, in_fmt, in_args...); }
    template <typename... Args>
    void info(const std::string& in_fmt, Args&&... in_args) const { _write(level::Info, in_fmt, in_args...); }

    void warn(const char * in_str) const { _write(level::Warn, in_str); }
    template <typename... Args>
    void warn(const char8_t* in_fmt, Args&&... in_args) const { _write(level::Warn, in_fmt, in_args...); }
    template <typename... Args>
    void warn(const char* in_fmt, Args&&... in_args) const { _write(level::Warn, in_fmt, in_args...); }
    template <typename... Args>
    void warn(const std::string& in_fmt, Args&&... in_args) const { _write(level::Warn, in_fmt, in_args...); }

    void error(const char * in_str) const { _write(level::Error, in_str); }
    template <typename... Args>
    void error(const char8_t* in_fmt, Args&&... in_args) const { _write(level::Error, in_fmt, in_args...); }
    template <typename... Args>
    void error(const char* in_fmt, Args&&... in_args) const { _write(level::Error, in_fmt, in_args...); }
    template <typename... Args>
    void error(const std::string& in_fmt, Args&&... in_args) const { _write(level::Error, in_fmt, in_args...); }

    void log(level in_level, const char * in_str) const { _write(in_level, in_str); }
    template <typename... Args>
    void log(level in_level, const char8_t* in_fmt, Args&&... in_args) const { _write(in_level, in_fmt, in_args...); }
    template <typename... Args>
    void log(level in_level, const char* in_fmt, Args&&... in_args) const { _write(in_level, in_fmt, in_args...); }
    template <typename... Args>
    void log(level in_level, const std::string& in_fmt, Args&&... in_args) const { _write(in_level, in_fmt, in_args...); }
};

};

template <>
struct std::formatter<core::console::level> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const core::console::level in_arg) {
        switch(in_arg) {
            case core::console::level::Debug:
                return "debug";
            case core::console::level::Info:
                return "info";
            case core::console::level::Warn:
                return "warn";
            case core::console::level::Error:
                return "error";
            default:
                return "unknown";
        }
    }
};

#endif // _CORE_CONSOLE_CONSOLE_HPP
