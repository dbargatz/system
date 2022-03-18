#include "node.hpp"
#include <cassert>
#include "__utils.hpp"

devicetree::node * devicetree::node::parse(const void * in_ptr, const void * in_strings_block) {
    auto n = new node();
    n->next = nullptr;
    n->_start = (struct internal::fdt_begin_node *)in_ptr;
    assert(internal::be_to_le(n->_start->token) == 0x00000001);
    n->_name = std::string_view(n->_start->name);
    if(n->_name.size() == 0) {
        n->_name = std::string_view("/");
    }

    auto align = internal::align(sizeof(*n->_start) + n->_name.size() + 1);
    auto next = (std::uint8_t *)in_ptr + align;
    node * prev_child = nullptr;
    property * prev_prop = nullptr;
    while(true) {
        auto token = internal::be_to_le(*(std::uint32_t *)next);
        switch(token) {
            case 0x01: {
                auto child = node::parse(next, in_strings_block);
                next += child->length();
                if(prev_child != nullptr) {
                    prev_child->next = child;
                } else {
                    n->_children = child;
                }
                prev_child = child;
                break;
            }
            case 0x02: {
                next += sizeof(std::uint32_t);
                n->_length = internal::align((std::size_t)(next - (std::uint8_t *)n->_start));
                return n;
            }
            case 0x03: {
                auto prop = property::parse(next, in_strings_block);
                next += prop->length();
                if(prev_prop != nullptr) {
                    prev_prop->next = prop;
                } else {
                    n->_properties = prop;
                }
                prev_prop = prop;
                break;
            }
            case 0x04: {
                next += sizeof(std::uint32_t);
                break;
            }
            default: {
                assert(false);
            }
        }
    }

    assert(false);
}

std::string devicetree::node::format(std::size_t in_indent) const {
    auto indent = std::string(in_indent * 2, ' ');
    auto str = std::format("{}{} {\n", indent, _name);

    auto prop = _properties;
    while(prop != nullptr) {
        auto prop_str = prop->format(in_indent+1);
        str.append(prop_str);
        prop = prop->next;
    }

    auto child = _children;
    while(child != nullptr) {
        auto child_str = child->format(in_indent+1);
        str.append(child_str);
        child = child->next;
    }
    auto close = std::format("{}} \n", indent);
    str.append(close);
    return str;
}

std::size_t devicetree::node::length() const {
    return _length;
}