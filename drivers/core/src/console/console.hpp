#ifndef _CORE_CONSOLE_CONSOLE_HPP
#define _CORE_CONSOLE_CONSOLE_HPP

#include <cstddef>
#include <cstdint>
#include <string>

#include "../platform.hpp"

namespace core::console {

class console {
public:
    console();

    void write(const char8_t* in_str);
    void write(const char* in_str) { write((const char8_t*)in_str); }
    void write(const std::string& in_str) { write(in_str.c_str()); }
};

};

#endif // _CORE_CONSOLE_CONSOLE_HPP
