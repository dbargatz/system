#include "stringlist.hpp"
#include <cstring>

template<>
devicetree::properties::string_iterator& devicetree::properties::string_iterator::operator++() {
    if(_current == nullptr) { return *this; }
    _current += std::strlen(_current) + 1;
    return *this;
}