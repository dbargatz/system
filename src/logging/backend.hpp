#ifndef _LOGGING_BACKEND_HPP
#define _LOGGING_BACKEND_HPP

#include <string.hpp>

namespace logging {

class backend {
public:
    virtual ~backend() noexcept = default;

    virtual void write(const std::string& in_str) = 0;
};

}; // namespace logging

#endif // _LOGGING_BACKEND_HPP
