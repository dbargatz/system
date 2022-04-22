#include "propertylist.hpp"
#include "property.hpp"

devicetree::property_iterator::property_iterator(struct internal::fdt_property * in_ptr) {
    _current = in_ptr;
}

bool devicetree::property_iterator::operator==(const property_iterator& in_other) const {
    return (_current == in_other._current);
}

bool devicetree::property_iterator::operator!=(const property_iterator& in_other) const {
    return !(*this == in_other);
}

devicetree::property devicetree::property_iterator::operator*() {
    return property(_current);
}

devicetree::property_iterator& devicetree::property_iterator::operator++() {
    // If this is an empty/sentinel property class, don't attempt to iterate.
    if(_current == nullptr) { return *this; }

    auto cur_prop = property(_current);
    auto next_token_ptr = (std::uint8_t *)_current + cur_prop.length();
    auto next_token = internal::be_to_le(*(std::uint32_t *)next_token_ptr);

    // Our options in a valid FDT are an optional sequence of NOPs followed by
    // an FDT_PROP token, or an optional sequence of NOPs followed by an
    // FDT_END_NODE. Because we could always encounter a sequence of NOPs, skip
    // over those first. For more info, see DeviceTree Spec v0.3, section 5.4.2.
    while(next_token == internal::FDT_NOP) {
        next_token_ptr += sizeof(next_token);
        next_token = internal::be_to_le(*(std::uint32_t *)next_token_ptr);
    }

    switch(next_token) {
        case internal::FDT_PROP: {
            // If the very next token is an FDT_PROP, that means there's a
            // a sibling property immediately after the current property, so
            // keep iterating!
            _current = (struct internal::fdt_property *)next_token_ptr;
            return *this;
        }
        case internal::FDT_BEGIN_NODE:
        case internal::FDT_END_NODE: {
            // If the very next token is an FDT_BEGIN_NODE or FDT_END_NODE, the
            // current property has no sibling after it, and we've iterated all
            // the properties in this node. Return the end() sentinel iterator.
            _current = nullptr;
            return *this;
        }
        default: {
            // Something has gone wrong - based on the DeviceTree Spec v0.3,
            // section 5.4.2, we can only have FDT_PROPs, FDT_NOPs, and
            // FDT_BEGIN_NODE/FDT_END_NODEs at the end of the node. We handle
            // each of these cases, so we've got a malformed FDT; blow up.
            assert(false);
        }
    }
}

devicetree::propertylist::propertylist() {
    _first_child = nullptr;
}

devicetree::propertylist::propertylist(struct internal::fdt_property * in_first_child) {
    _first_child = in_first_child;
}

devicetree::propertylist::iterator devicetree::propertylist::begin() noexcept {
    return property_iterator(_first_child);
}

devicetree::propertylist::iterator devicetree::propertylist::end() noexcept {
    return property_iterator(nullptr);
}