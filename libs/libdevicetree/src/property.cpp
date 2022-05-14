#include "property.hpp"
#include <cassert>
#include "properties/stringlist.hpp"

using namespace std::literals;


const std::uint8_t * devicetree::property::_s_strings_block;


devicetree::property::property(const void * in_ptr) {
    _start = (property::internal_struct *)in_ptr;
    assert(details::be_to_host(_start->token) == details::FDT_PROP);

    auto name_ptr = (const char *)_s_strings_block + details::be_to_host(_start->nameoff);
    _name = std::string_view(name_ptr);
}

template<> char * devicetree::property::get_value<char *>(std::uint32_t in_offset) const {
    auto len = details::be_to_host(_start->len);
    assert(in_offset < len);
    auto ptr = _start->value + in_offset;
    return (char *)ptr;
}

template<> devicetree::properties::stringlist devicetree::property::get_value<devicetree::properties::stringlist>(std::uint32_t in_offset) const {
    auto len = details::be_to_host(_start->len);
    assert(in_offset < len);
    auto ptr = _start->value + in_offset;
    return devicetree::properties::stringlist(ptr, len - in_offset);
}

template<> std::uint32_t devicetree::property::get_value<std::uint32_t>(std::uint32_t in_offset) const {
    auto len = details::be_to_host(_start->len);
    assert(in_offset <= (len - sizeof(std::uint32_t)));
    assert((in_offset % sizeof(std::uint32_t)) == 0);
    auto ptr = _start->value + in_offset;
    return details::be_to_host(*(std::uint32_t *)ptr);
}

template<> std::uint64_t devicetree::property::get_value<std::uint64_t>(std::uint32_t in_offset) const {
    auto len = details::be_to_host(_start->len);
    assert(in_offset <= (len - sizeof(std::uint64_t)));
    assert((in_offset % sizeof(std::uint64_t)) == 0);
    auto ptr = _start->value + in_offset;
    return details::be_to_host(*(std::uint64_t *)ptr);
}

template<> std::string_view devicetree::property::get_value<std::string_view>(std::uint32_t in_offset) const {
    auto len = details::be_to_host(_start->len);
    assert(in_offset < len);
    auto ptr = _start->value + in_offset;
    return std::string_view((const char *)ptr);
}

std::string devicetree::property::format(std::size_t in_indent) const {
    auto indent = std::string(in_indent * 2, ' ');
    if(_name == "model"sv ||
       _name == "status"sv ||
       _name == "name"sv ||
       _name == "device_type"sv ||
       _name == "bootargs"sv ||
       _name == "stdout-path"sv ||
       _name == "stdin-path"sv ||
       _name == "power-isa-version"sv ||
       _name == "mmu-type"sv ||
       _name == "label"sv
    ) {
        auto value = get_value<std::string_view>();
        return std::format("{}{}: {}\n", indent, _name, value);
    }
    
    if(_name == "phandle"sv ||
       _name == "interrupt-parent"sv ||
       _name == "#address-cells"sv ||
       _name == "#size-cells"sv ||
       _name == "#interrupt-cells"sv ||
       _name == "virtual-reg"sv ||
       _name == "cache-op-block-size"sv ||
       _name == "reservation-granule-size"sv ||
       _name == "tlb-size"sv ||
       _name == "tlb-sets"sv ||
       _name == "d-tlb-size"sv ||
       _name == "d-tlb-sets"sv ||
       _name == "i-tlb-size"sv ||
       _name == "i-tlb-sets"sv ||
       _name == "cache-size"sv ||
       _name == "cache-sets"sv ||
       _name == "cache-block-size"sv ||
       _name == "cache-line-size"sv ||
       _name == "i-cache-size"sv ||
       _name == "i-cache-sets"sv ||
       _name == "i-cache-block-size"sv ||
       _name == "i-cache-line-size"sv ||
       _name == "d-cache-size"sv ||
       _name == "d-cache-sets"sv ||
       _name == "d-cache-block-size"sv ||
       _name == "d-cache-line-size"sv ||
       _name == "next-level-cache"sv ||
       _name == "cache-level"sv ||
       _name == "reg-shift"sv
    ) {
        auto value = get_value<std::uint32_t>();
        return std::format("{}{}: {}\n", indent, _name, value);
    }
    
    if(_name == "cpu-release-addr"sv) {
        auto value = get_value<std::uint64_t>();
        return std::format("{}{}: {}\n", indent, _name, value);
    } 
    
    if(_name == "compatible"sv ||
       _name == "enable-method"sv
    ) {
        auto value = get_value<devicetree::properties::stringlist>();
        auto str = std::format("{}{}:\n", indent, _name);
        for(auto&& item : value) {
            auto line = std::format("{}  - {}\n", indent, item);
            str.append(line);
        }
        return str;
    }

    return std::format("{}{}: {}\n", indent, _name, "???"sv);
}

std::size_t devicetree::property::length() const {
    auto len = sizeof(*_start) + details::be_to_host(_start->len);
    return details::align(len);
}

std::string_view devicetree::property::name() const {
    return _name;
}

template<>
devicetree::property_iterator& devicetree::property_iterator::operator++() {
    // If this is an empty/sentinel property class, don't attempt to iterate.
    if(_current == nullptr) { return *this; }

    auto cur_prop = property(_current);
    auto next_token_ptr = (std::uint8_t *)_current + cur_prop.length();
    auto next_token = details::be_to_host(*(std::uint32_t *)next_token_ptr);

    // Our options in a valid FDT are an optional sequence of NOPs followed by
    // an FDT_PROP token, or an optional sequence of NOPs followed by an
    // FDT_END_NODE. Because we could always encounter a sequence of NOPs, skip
    // over those first. For more info, see DeviceTree Spec v0.3, section 5.4.2.
    while(next_token == details::FDT_NOP) {
        next_token_ptr += sizeof(next_token);
        next_token = details::be_to_host(*(std::uint32_t *)next_token_ptr);
    }

    switch(next_token) {
        case details::FDT_PROP: {
            // If the very next token is an FDT_PROP, that means there's a
            // a sibling property immediately after the current property, so
            // keep iterating!
            _current = (property::internal_struct *)next_token_ptr;
            return *this;
        }
        case details::FDT_BEGIN_NODE:
        case details::FDT_END_NODE: {
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