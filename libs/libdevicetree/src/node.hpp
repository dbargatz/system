#ifndef _DEVICETREE_NODE_HPP
#define _DEVICETREE_NODE_HPP

#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include "__structs.hpp"
#include "nodelist.hpp"
#include "property.hpp"

namespace devicetree {

class node {
private:
    struct internal::fdt_begin_node * _start;
    std::string_view _name;

public:
    node();
    node(const void * in_ptr);

    bool find(std::string_view in_name, node * out_node);
    bool operator==(const node& in_other) const { return _start == in_other._start; }

    nodelist children();

    std::string format(std::size_t in_indent = 0) const;
    std::size_t length() const;
}; // class node

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