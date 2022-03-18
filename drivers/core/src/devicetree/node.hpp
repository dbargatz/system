#ifndef _CORE_DEVICETREE_NODE_HPP
#define _CORE_DEVICETREE_NODE_HPP

#include <cassert>
#include <cstdint>
#include <string_view>
#include "utils.hpp"

namespace core::devicetree {

class node {
private:
    const std::uint32_t * _start;

public:
    node(const std::uint32_t * in_token) : _start(in_token) {
        auto token = be_to_le(*in_token);
        assert(token == 0x00000001);
    }

    constexpr std::size_t length() {
        auto depth = 0;
        auto next = next_token(_start);
        while(true) {
            auto token = be_to_le(*next);
            switch(be_to_le(*next)) {
                case 0x01: {
                    depth++;
                    break;
                }
                case 0x02: {
                    if(depth == 0) { 
                        return ((std::uint8_t *)next - (std::uint8_t *)_start) + 4;
                    }
                    depth--;
                    break;
                }
                default:
                    break;
            }
            next = next_token(next);
            assert(next != nullptr);
        }
    }

    constexpr const std::uint32_t * next_token(const std::uint32_t * in_cur_token) {
        auto token = be_to_le(*in_cur_token);
        switch(token) {
            case 0x01: {
                // BEGIN_NODE token, skip over token and name.
                auto name = std::string_view((const char *)in_cur_token + sizeof(*in_cur_token));
                auto next = (const std::uint8_t *)_start + align_to(4, name.size() + 1);
                return (std::uint32_t *)next;
            }
            case 0x02: {
                // END_NODE token, simply skip over token.
                return in_cur_token++;
            }
            case 0x03: {
                // PROPERTY token, skip over the property structure and value.
                auto len = be_to_le(*(in_cur_token + sizeof(*in_cur_token)));
                auto next = (const std::uint8_t *)in_cur_token + (sizeof(*in_cur_token) * 3) + len;
                return (std::uint32_t *)next;
            }
            case 0x04: {
                // NOP token, simply skip over token.
                return in_cur_token++;
            }
            case 0x09: {
                // END token, return nullptr.
                return nullptr;
            }
            default: {
                assert(false);
            }
        }
    }

    node * next_child(node * in_cur_child) {
        if(in_cur_child == nullptr) {

        }
        auto cur_token_ptr = (in_cur_child == nullptr) ? _start : in_cur_child->_start;
        auto next_token_ptr = next_token(cur_token_ptr);
        auto depth = 1;
        while(depth > 0 && be_to_le(*next_token_ptr))

        if(in_cur_child == nullptr) {
            in_cur_child = node(_start);
        }
        auto next = next_token(in_cur_child);
        auto name = std::string_view((const char *)_start + sizeof(*_start));
        auto next = (const std::uint32_t *)((const std::uint8_t *)_start + align_to(4, name.size() + 1));

    }

    const node * find(std::string_view in_path) const {
        // Grab the node's name and ensure the name length is within bounds.
        // DTS v0.3, 2.2.1: The node name must be 1 to 31 characters in length.
        auto name = std::string_view((const char *)_start + sizeof(*_start));
        assert(1 <= name.size() && name.size() <= 31);

        if(!in_path.starts_with(name)) { return nullptr; }
        in_path.remove_prefix(name.size());
        auto child = next_child(nullptr);
        while(child != nullptr) {
            auto result = child->find(in_path);
            if(result != nullptr) {
                return result;
            }
            child = next_child(child);
        }

        return nullptr;

        auto next = (const std::uint32_t *)((const std::uint8_t *)_start + align_to(4, name.size() + 1));
        while(true) {
            auto token = be_to_le(*next);
            switch(token) {
                case 0x01: {
                    // BEGIN_NODE, search child.
                    auto child = (const node *)next;
                    auto find_res = child->find(in_path);
                    if(find_res != nullptr) {
                        return find_res;
                    }
                    break;
                }
                case 0x02: {
                    // END_NODE, no more children, not found
                    return nullptr;
                }
                case 0x03: {
                    // Property, not interested
                    break;
                }
                case 0x04: {
                    // NOP, continue
                    break;
                }
                case 0x09: {
                    // END, no more children, not found
                    break;
                }
                default: {
                    assert(false);
                }
            }
        }

        // We know the path is non-empty, so advance to the next path element, or to the end of the path.
        auto subpath = in_path;
        while(*subpath != '/') {
            if(*subpath == '\0') {

            }
            subpath++;
        }


        //     root.find(/cpus/cpu@3)
        //       cpus.find(cpu@3)
        //          cpu@3.find('')


        // Check properties.

        // Check child nodes.
    }
};

};

#endif // _CORE_DEVICETREE_NODE_HPP