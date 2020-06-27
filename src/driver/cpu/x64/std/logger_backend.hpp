#ifndef _STD_LOGGER_BACKEND_HPP
#define _STD_LOGGER_BACKEND_HPP

#include "stdint.h"
#include "text.hpp"

/**
 * Abstract class. Must be implemented by classes which log output is sent to.
 */
class logger_backend {
public:
    virtual void debug(const text& in_msg) = 0;
    virtual void error(const text& in_msg) = 0;
    virtual void info(const text& in_msg)  = 0;
    virtual void panic(const text& in_msg) = 0;
    virtual void warn(const text& in_msg)  = 0;
};

#endif // _STD_LOGGER_BACKEND_HPP
