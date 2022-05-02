#ifndef _DEVICETREE_PROPERTIES_STRINGLIST_HPP
#define _DEVICETREE_PROPERTIES_STRINGLIST_HPP

#include <cstddef>
#include "../__list.hpp"
#include "../__iterator.hpp"

namespace devicetree::properties {

using string_iterator = details::iterator<const char *, const char>;

class stringlist : public details::list<const char *, const char> {
public:
    stringlist() { _first = nullptr; _size = 0; }
    stringlist(const char * in_first, std::size_t in_size) { _first = in_first; _size = in_size; }

    string_iterator begin() noexcept { return string_iterator(_first); }
    string_iterator end() noexcept { return _first + _size; }

private:
    const char * _first;
    std::size_t _size;
}; // class stringlist

}; // namespace devicetree::properties

namespace devicetree {

template <>
properties::string_iterator& properties::string_iterator::operator++();

};

#endif // _DEVICETREE_PROPERTIES_STRINGLIST_HPP