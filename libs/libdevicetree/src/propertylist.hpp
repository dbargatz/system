#ifndef _DEVICETREE_PROPERTYLIST_HPP
#define _DEVICETREE_PROPERTYLIST_HPP

#include <cstdint>
#include "__structs.hpp"
#include "__utils.hpp"
#include "property.hpp"

namespace devicetree {

class property_iterator {
private:
    struct internal::fdt_property * _current;

public:
    property_iterator(struct internal::fdt_property * in_ptr);
    bool operator==(const property_iterator& in_other) const;
    bool operator!=(const property_iterator& in_other) const;

    property operator*();
    property_iterator& operator++();
};

class propertylist {
private:
    struct internal::fdt_property * _first_child;

public:
    using iterator = property_iterator;

    propertylist();
    propertylist(struct internal::fdt_property * in_first_child);

    iterator begin() noexcept;
    iterator end() noexcept;
};

}; // namespace devicetree

#endif // _DEVICETREE_PROPERTYLIST_HPP