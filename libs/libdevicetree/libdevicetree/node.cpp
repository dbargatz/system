#include "node.hpp"
#include <cassert>
#include "__utils.hpp"
#include "nodelist.hpp"

devicetree::node::node() {
    _start = (struct internal::fdt_begin_node *)nullptr;
    _name = std::string_view("invalid");
}

devicetree::node::node(const void * in_ptr) {
    _start = (struct internal::fdt_begin_node *)in_ptr;
    assert(internal::be_to_le(_start->token) == internal::FDT_BEGIN_NODE);
    _name = std::string_view(_start->name);
}

bool devicetree::node::find(std::string_view in_name, devicetree::node * out_node) {
    if(in_name.starts_with("/")) {
        in_name.remove_prefix(1);
    }

    auto at_idx = _name.find('@');
    auto cur_name = _name;
    if(at_idx != std::string_view::npos) {
        cur_name = _name.substr(0, at_idx);
    }

    if(in_name == cur_name) {
        out_node->_start = _start;
        out_node->_name = _name;
        return true; 
    }
    else if(!in_name.starts_with(cur_name)) {
        return false;
    }

    in_name.remove_prefix(cur_name.length());
    auto align = internal::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + align;
    while(true) {
        auto cur_token = internal::be_to_le(*(std::uint32_t *)next);
        switch(cur_token) {
            case internal::FDT_BEGIN_NODE: {
                auto child_node = node(next);
                auto result = child_node.find(in_name, out_node);
                if(result) {
                    return true;
                }

                next += child_node.length();
                break;
            }
            case internal::FDT_END_NODE: {
                return false;
            }
            case internal::FDT_PROP: {
                auto prop = property(next);
                next += prop.length();
                break;
            }
            case internal::FDT_NOP: {
                next += sizeof(std::uint32_t);
                break;
            }
            default: {
                assert(false);
            }
        }
    }
}

std::string devicetree::node::format(std::size_t in_indent) const {
    auto indent = std::string(in_indent * 2, ' ');
    auto str = std::format("{}{} {\n", indent, _name);

    auto aligned_len = internal::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + aligned_len;
    while(true) {
        auto cur_token = internal::be_to_le(*(std::uint32_t *)next);
        switch(cur_token) {
            case internal::FDT_BEGIN_NODE: {
                auto child_node = node(next);
                str.append(child_node.format(in_indent + 1));
                next += child_node.length();
                break;
            }
            case internal::FDT_END_NODE: {
                auto close_brace = std::format("{}}\n", indent);
                str.append(close_brace);
                return str;
            }
            case internal::FDT_PROP: {
                auto prop = property(next);
                str.append(prop.format(in_indent + 1));
                next += prop.length();
                break;
            }
            case internal::FDT_NOP: {
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
    auto aligned_len = internal::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + aligned_len;
    while(true) {
        auto cur_token = internal::be_to_le(*(std::uint32_t *)next);
        switch(cur_token) {
            case internal::FDT_BEGIN_NODE: {
                auto child_len = node(next).length();
                aligned_len += child_len;
                next += child_len;
                break;
            }
            case internal::FDT_END_NODE: {
                return aligned_len + sizeof(std::uint32_t);
            }
            case internal::FDT_PROP: {
                auto prop_len = property(next).length();
                aligned_len += prop_len;
                next += prop_len;
                break;
            }
            case internal::FDT_NOP: {
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

devicetree::propertylist devicetree::node::properties() {
    auto aligned_len = internal::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + aligned_len;
    while(true) {
        auto cur_token = internal::be_to_le(*(std::uint32_t *)next);
        switch(cur_token) {
            case internal::FDT_BEGIN_NODE:
            case internal::FDT_END_NODE: {
                // If we encounter an FDT_BEGIN_NODE or an FDT_END_NODE, we've
                // run out of properties in this node, according to the spec.
                // Return an empty propertylist.
                return propertylist();
            }
            case internal::FDT_PROP: {
                // We found the first property in this node's set of properties;
                // return a propertylist containing the first property.
                auto prop = (struct internal::fdt_property *)next;
                return propertylist(prop);
            }
            case internal::FDT_NOP: {
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

devicetree::nodelist devicetree::node::children() {
    auto aligned_len = internal::align(sizeof(*_start) + _name.size() + 1);
    auto next = (std::uint8_t *)_start + aligned_len;
    while(true) {
        auto cur_token = internal::be_to_le(*(std::uint32_t *)next);
        switch(cur_token) {
            case internal::FDT_BEGIN_NODE: {
                // The first FDT_BEGIN_NODE we encounter before an FDT_END_NODE
                // (if any) is the first child, so create and return the
                // nodelist from it.
                auto first_child = (struct internal::fdt_begin_node *)next;
                return nodelist(first_child);
            }
            case internal::FDT_END_NODE: {
                // If we encounter an FDT_END_NODE before an FDT_BEGIN_NODE,
                // this node has no child nodes, so return an empty nodelist.
                return nodelist();
            }
            case internal::FDT_PROP: {
                // We're not interested in properties, so move past any we find.
                auto prop_len = property(next).length();
                next += prop_len;
                break;
            }
            case internal::FDT_NOP: {
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