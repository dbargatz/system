#include "node.hpp"
#include <cassert>
#include "__utils.hpp"

devicetree::node::node(const void * in_ptr) {
    _start = (struct details::fdt_begin_node *)in_ptr;
    assert(details::be_to_host(_start->token) == details::FDT_BEGIN_NODE);
    _name = std::string_view(_start->name);
}

std::expected<devicetree::node, std::uint32_t> devicetree::node::get_node(const char * in_path) {
    auto path = std::string_view(in_path);
    if(path.starts_with("/")) { path.remove_prefix(1); }

    auto at_idx = _name.find('@');
    auto cur_name = _name;
    if(at_idx != std::string_view::npos) {
        cur_name = _name.substr(0, at_idx);
    }

    if(path == cur_name) {
        return *this;
    } else if(!path.starts_with(cur_name)) {
        return std::unexpected(0);
    }

    for(auto&& child : children()) {
        auto result = child.get_node(path.data());
        if(result) { return result; }
    }

    return std::unexpected(0);
}

std::expected<devicetree::property, std::uint32_t> devicetree::node::get_property(const char * in_name) {
    auto target_prop = std::string_view(in_name);
    for(auto&& prop : properties()) {
        if(prop.name() == target_prop) {
            return prop;
        }
    }
    return std::unexpected(0);
}

std::string devicetree::node::format(std::size_t in_indent) const {
    auto indent = std::string(in_indent * 2, ' ');
    auto str = std::format("{}{} {\n", indent, _name);

    auto aligned_len = details::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + aligned_len;
    while(true) {
        auto cur_token = details::be_to_host(*(std::uint32_t *)next);
        switch(cur_token) {
            case details::FDT_BEGIN_NODE: {
                auto child_node = node(next);
                str.append(child_node.format(in_indent + 1));
                next += child_node.length();
                break;
            }
            case details::FDT_END_NODE: {
                auto close_brace = std::format("{}}\n", indent);
                str.append(close_brace);
                return str;
            }
            case details::FDT_PROP: {
                auto prop = property(next);
                str.append(prop.format(in_indent + 1));
                next += prop.length();
                break;
            }
            case details::FDT_NOP: {
                next += sizeof(std::uint32_t);
                break;
            }
            default: {
                assert(false);
            }
        }
    }
}

std::size_t devicetree::node::length() const {
    auto aligned_len = details::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + aligned_len;
    while(true) {
        auto cur_token = details::be_to_host(*(std::uint32_t *)next);
        switch(cur_token) {
            case details::FDT_BEGIN_NODE: {
                auto child_len = node(next).length();
                aligned_len += child_len;
                next += child_len;
                break;
            }
            case details::FDT_END_NODE: {
                return aligned_len + sizeof(std::uint32_t);
            }
            case details::FDT_PROP: {
                auto prop_len = property(next).length();
                aligned_len += prop_len;
                next += prop_len;
                break;
            }
            case details::FDT_NOP: {
                aligned_len += sizeof(std::uint32_t);
                next += sizeof(std::uint32_t);
                break;
            }
            default: {
                assert(false);
            }
        }
    }
}

devicetree::details::list<devicetree::property, struct devicetree::details::fdt_property> devicetree::node::properties() {
    auto aligned_len = details::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + aligned_len;
    while(true) {
        auto cur_token = details::be_to_host(*(std::uint32_t *)next);
        switch(cur_token) {
            case details::FDT_BEGIN_NODE:
            case details::FDT_END_NODE: {
                // If we encounter an FDT_BEGIN_NODE or an FDT_END_NODE, we've
                // run out of properties in this node, according to the spec.
                // Return an empty propertylist.
                return details::list<property, struct details::fdt_property>();
            }
            case details::FDT_PROP: {
                // We found the first property in this node's set of properties;
                // return a propertylist containing the first property.
                auto prop = (property::internal_struct *)next;
                return details::list<property, struct details::fdt_property>(prop);
            }
            case details::FDT_NOP: {
                // We're not interested in NOPs, so move past any we find.
                next += sizeof(std::uint32_t);
                break;
            }
            default: {
                // Something is wrong - we've encountered an FDT_END token or a
                // completely invalid token. Blow up.
                assert(false);
            }
        }
    }
}

devicetree::details::list<devicetree::node, struct devicetree::details::fdt_begin_node> devicetree::node::children() {
    auto aligned_len = details::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + aligned_len;
    while(true) {
        auto cur_token = details::be_to_host(*(std::uint32_t *)next);
        switch(cur_token) {
            case details::FDT_BEGIN_NODE: {
                // The first FDT_BEGIN_NODE we encounter before an FDT_END_NODE
                // (if any) is the first child, so create and return the
                // nodelist from it.
                auto first_child = (struct details::fdt_begin_node *)next;
                return details::list<node, struct details::fdt_begin_node>(first_child);
            }
            case details::FDT_END_NODE: {
                // If we encounter an FDT_END_NODE before an FDT_BEGIN_NODE,
                // this node has no child nodes, so return an empty nodelist.
                return details::list<node, struct details::fdt_begin_node>();
            }
            case details::FDT_PROP: {
                // We're not interested in properties, so move past any we find.
                auto prop_len = property(next).length();
                next += prop_len;
                break;
            }
            case details::FDT_NOP: {
                // We're not interested in NOPs, so move past any we find.
                next += sizeof(std::uint32_t);
                break;
            }
            default: {
                // Something is wrong - we've encountered an FDT_END token or a
                // completely invalid token. Blow up.
                assert(false);
            }
        }
    }
}

template<>
devicetree::node_iterator& devicetree::node_iterator::operator++() {
    if(_current == nullptr) { return *this; }

    auto cur = node(_current);
    auto next_token_ptr = (std::uint8_t *)_current + cur.length();
    auto next_token = details::be_to_host(*(std::uint32_t *)next_token_ptr);
    switch(next_token) {
        case details::FDT_BEGIN_NODE: {
            // If the very next token is a begin node, that means there's
            // a sibling node immediately after the current node, so keep
            // iterating!
            _current = (struct details::fdt_begin_node *)next_token_ptr;
            return *this;
        }
        case details::FDT_NOP:
        case details::FDT_END_NODE: {
            // If the very next token is an FDT_END_NODE or an FDT_NOP, that
            // means the current child node has no sibling after it, and
            // we've iterated all the children of the parent node. Return
            // the end() sentinel iterator.
            _current = nullptr;
            return *this;
        }
        default: {
            // Something has gone wrong - based on the DeviceTree Spec v0.3,
            // section 5.4.2, we can only have child node representations,
            // FDT_NOPs, and FDT_END_NODEs at the end of the parent node.
            // We handle each of these cases, so we've got a malformed FDT;
            // time to blow up.
            assert(false);
        }
    }
}