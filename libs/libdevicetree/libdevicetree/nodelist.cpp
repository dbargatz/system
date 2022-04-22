#include "nodelist.hpp"
#include "node.hpp"

devicetree::node_iterator::node_iterator(struct internal::fdt_begin_node * in_ptr) {
    _current = in_ptr;
}

bool devicetree::node_iterator::operator==(const node_iterator& in_other) const {
    return (_current == in_other._current);
}

bool devicetree::node_iterator::operator!=(const node_iterator& in_other) const {
    return !(*this == in_other);
}

devicetree::node devicetree::node_iterator::operator*() {
    return _current;
}

devicetree::node_iterator& devicetree::node_iterator::operator++() {
    if(_current == nullptr) { return *this; }

    auto cur_node = node(_current);
    auto next_token_ptr = (std::uint8_t *)_current + cur_node.length();
    auto next_token = internal::be_to_le(*(std::uint32_t *)next_token_ptr);
    switch(next_token) {
        case internal::FDT_BEGIN_NODE: {
            // If the very next token is a begin node, that means there's
            // a sibling node immediately after the current node, so keep
            // iterating!
            _current = (struct internal::fdt_begin_node *)next_token_ptr;
            return *this;
        }
        case internal::FDT_NOP:
        case internal::FDT_END_NODE: {
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

devicetree::nodelist::nodelist() {
    _first_child = nullptr;
}

devicetree::nodelist::nodelist(struct internal::fdt_begin_node * in_first_child) {
    _first_child = in_first_child;
}

devicetree::nodelist::iterator devicetree::nodelist::begin() noexcept {
    return node_iterator(_first_child);
}

devicetree::nodelist::iterator devicetree::nodelist::end() noexcept {
    return node_iterator(nullptr);
}