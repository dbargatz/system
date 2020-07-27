#ifndef _LOADER_BINARY_HPP
#define _LOADER_BINARY_HPP

#include "../logging/logger.hpp"

namespace loader {

class binary {
public:
    binary(logging::logger& in_logger) : _log(in_logger) {}
    bool init(const void * in_start_addr, const void * in_end_addr);

private:
    logging::logger& _log;
};

}; //namespace loader

#endif // _LOADER_BINARY_HPP
