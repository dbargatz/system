#include "node.hpp"
#include <cassert>
#include "__utils.hpp"

devicetree::node::node(const void * in_ptr, const void * in_strings_block) {
    _start = (struct internal::fdt_begin_node *)in_ptr;
    assert(internal::be_to_le(_start->token) == 0x00000001);
    _strings_block = (std::uint8_t *)in_strings_block;

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
        out_node->_strings_block = _strings_block;
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
                auto child_node = node(next, _strings_block);
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
                auto prop = property(next, _strings_block);
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
                auto child_node = node(next, _strings_block);
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
                auto prop = property(next, _strings_block);
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
                auto child_len = node(next, _strings_block).length();
                aligned_len += child_len;
                next += child_len;
                break;
            }
            case internal::FDT_END_NODE: {
                return aligned_len + sizeof(std::uint32_t);
            }
            case internal::FDT_PROP: {
                auto prop_len = property(next, _strings_block).length();
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