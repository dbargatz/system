#ifndef _DEVICETREE_NODE_HPP
#define _DEVICETREE_NODE_HPP

#define LIBCXX_DISABLE_EXCEPTIONS
#define LIBCXX_DISABLE_MEM_FN

#include <cstdint>
#include <expected>
#include <format>
#include <string>
#include <string_view>
#include "__list.hpp"
#include "__structs.hpp"
#include "property.hpp"

namespace devicetree {

class node {
public:
    using internal_struct = struct details::fdt_begin_node;

    node() = delete;
    node(const void * in_ptr);

    std::expected<node, std::uint32_t> get(const char * in_path);

    template <typename T>
    std::expected<T, std::uint32_t> get(const char * in_property) {
        auto target_prop = std::string_view(in_property);
        for(auto&& prop : properties()) {
            if(prop.name() == target_prop) {
                return prop.get_value<T>();
            }
        }
        return std::unexpected(0);
    }

    bool operator==(const node& in_other) const { return _start == in_other._start; }

    details::list<node, struct details::fdt_begin_node> children();
    property_list properties();

    std::string format(std::size_t in_indent = 0) const;
    std::size_t length() const;

private:
    node::internal_struct * _start;
    std::string_view _name;
}; // class node

using node_iterator = details::iterator<node, struct details::fdt_begin_node>;
using node_list = details::list<node, struct details::fdt_begin_node>;

template <>
node_iterator& node_iterator::operator++();

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