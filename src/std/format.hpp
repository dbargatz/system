#ifndef _STD_FORMAT_HPP
#define _STD_FORMAT_HPP

#include <string.hpp>

namespace std {

template <typename... Args>
std::string format(std::string in_fmt, const Args&... in_args) {
    return in_fmt;
}

}; // namespace std

#endif // _STD_FORMAT_HPP
