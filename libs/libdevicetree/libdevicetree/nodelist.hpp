#ifndef _DEVICETREE_NODELIST_HPP
#define _DEVICETREE_NODELIST_HPP

#include <cstdint>
#include "__structs.hpp"
#include "__utils.hpp"

namespace devicetree {

class node;

class node_iterator {
private:
    struct internal::fdt_begin_node * _current;

public:
    node_iterator(struct internal::fdt_begin_node * in_ptr);
    bool operator==(const node_iterator& in_other) const;
    bool operator!=(const node_iterator& in_other) const;

    node operator*();
    node_iterator& operator++();
};

class nodelist {
private:
    struct internal::fdt_begin_node * _first_child;

public:
    using iterator = node_iterator;

    nodelist();
    nodelist(struct internal::fdt_begin_node * in_first_child);

    iterator begin() noexcept;
    iterator end() noexcept;
};

}; // namespace devicetree

#endif // _DEVICETREE_NODELIST_HPP