#ifndef _DEVICETREE_NODE_HPP
#define _DEVICETREE_NODE_HPP

#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include "__list.hpp"
#include "__structs.hpp"
#include "property.hpp"

namespace devicetree {

class node {
private:
    struct internal::fdt_begin_node * _start;
    std::string_view _name;

public:
    using internal_struct = struct internal::fdt_begin_node;

    node();
    node(const void * in_ptr);

    details::list<node> find(std::string_view in_node_path);
    bool operator==(const node& in_other) const { return _start == in_other._start; }

    details::list<node> children();
    details::list<property> properties();

    std::string format(std::size_t in_indent = 0) const;
    std::size_t length() const;
}; // class node

template <>
details::iterator<node>& details::iterator<node>::operator++();

}; // namespace devicetree

template <>
struct std::formatter<devicetree::node> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const devicetree::node& in_arg) {
        return in_arg.format();
    }
}; // struct std::formatter

#endif // _DEVICETREE_NODE_HPP