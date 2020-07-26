#ifndef _LOADER_BINARY_HPP
#define _LOADER_BINARY_HPP

namespace loader {

class binary {
public:
    bool init(const void * in_start_addr, const void * in_end_addr);
};

}; //namespace loader

#endif // _LOADER_BINARY_HPP
