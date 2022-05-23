#ifndef _DEVICETREE_NODE_HPP
#define _DEVICETREE_NODE_HPP

#define LIBCXX_DISABLE_EXCEPTIONS
#define LIBCXX_DISABLE_MEM_FN

#include <algorithm>
#include <cstdint>
#include <expected>
#include <format>
#include <string>
#include <string_view>
#include "__list.hpp"
#include "__structs.hpp"
#include "property.hpp"
#include "properties/ranges.hpp"
#include "properties/reg.hpp"

namespace devicetree {

template <typename T>
concept Property = std::is_convertible_v<T, property>;

class node {
public:
    node() = delete;
    node(const void * in_ptr);

    std::expected<node, std::uint32_t> get_node(const char * in_path);
    std::expected<property, std::uint32_t> get_property(const char * in_name);

    template<Property T = property>
    std::expected<T, std::uint32_t> get_property(const char * in_property_name, std::uint32_t in_parent_addr_cells, std::uint32_t in_parent_size_cells, std::uint32_t in_child_addr_cells, std::uint32_t in_child_size_cells) {
        auto prop = get_property(in_property_name);
        if(!prop) { return std::unexpected(0); }
        return T(*prop, in_parent_addr_cells, in_parent_size_cells, in_child_addr_cells, in_child_size_cells);
    }

    std::expected<properties::ranges, std::uint32_t> get_ranges_property(std::uint32_t in_parent_addr_cells, std::uint32_t in_parent_size_cells, std::uint32_t in_child_addr_cells, std::uint32_t in_child_size_cells) {
        return get_property<properties::ranges>("ranges", in_parent_addr_cells, in_parent_size_cells, in_child_addr_cells, in_child_size_cells);
    }

    std::expected<properties::reg, std::uint32_t> get_reg_property(std::uint32_t in_addr_cells, std::uint32_t in_size_cells) {
        return get_property<properties::reg>("reg", in_addr_cells, in_size_cells, 0, 0);
    }

    template <typename T>
    std::expected<T, std::uint32_t> get_value(const char * in_property_name, std::uint32_t in_offset = 0) {
        auto prop = get_property(in_property_name);
        if(!prop) { return std::unexpected(0); }

        return prop->get_value<T>(in_offset);
    }

    template <typename P>
    std::expected<node, std::uint32_t> find(P in_predicate) {
        if(in_predicate(*this)) { return *this; }

        for(auto&& child : children()) {
            auto result = child.find(in_predicate);
            if(result) { return result; }
        }

        return std::unexpected(0);
    }

    bool operator==(const node& in_other) const { return _start == in_other._start; }

    details::list<node, struct details::fdt_begin_node> children();
    property_list properties();

    std::string format(std::size_t in_indent = 0) const;
    std::size_t length() const;

private:
    struct details::fdt_begin_node * _start;
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