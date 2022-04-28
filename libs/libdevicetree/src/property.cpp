#include "property.hpp"
#include <cassert>

using namespace std::literals;


const std::uint8_t * devicetree::property::_s_strings_block;


devicetree::property::property(const void * in_ptr) {
    _start = (property::internal_struct *)in_ptr;
    assert(details::be_to_host(_start->token) == details::FDT_PROP);

    auto name_ptr = (const char *)_s_strings_block + details::be_to_host(_start->nameoff);
    _name = std::string_view(name_ptr);
}

template<> std::uint32_t devicetree::property::get_value<std::uint32_t>() const {
    assert(details::be_to_host(_start->len) == sizeof(std::uint32_t));
    return details::be_to_host(*(std::uint32_t *)_start->value);
}

template<> std::uint64_t devicetree::property::get_value<std::uint64_t>() const {
    assert(details::be_to_host(_start->len) == sizeof(std::uint64_t));
    return details::be_to_host(*(std::uint64_t *)_start->value);
}

template<> std::string_view devicetree::property::get_value<std::string_view>() const {
    return std::string_view((const char *)_start->value);
}

template<> std::vector<std::string_view> devicetree::property::get_value<std::vector<std::string_view>>() const {
    auto vec = std::vector<std::string_view>();
    const char * str_ptr = _start->value;
    const char * end_ptr = _start->value + details::be_to_host(_start->len);
    while(str_ptr < end_ptr) {
        auto view = std::string_view(str_ptr);
        vec.push_back(view);
        str_ptr += view.length() + 1;
    }
    return vec;
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
        auto value = get_value<std::vector<std::string_view>>();
        auto str = std::format("{}{}:\n", indent, _name);
        for(auto&& item : value) {
            auto line = std::format("{}  - {}\n", indent, item);
            str.append(line);
        }
        return str;
    }

    if(_name == "reg"sv) {
        auto value = get_prop_encoded_array<struct details::fdt_reg>();
        auto str = std::format("{}{}:\n", indent, _name);
        for(auto&& item : value) {
            auto start = details::be_to_host(item->address);
            auto len = details::be_to_host(item->length);
            auto end = start + len;
            auto line = std::format("{}  - 0x{:X} - 0x{:X} ({} bytes)\n", indent, start, end, len);
            str.append(line);
        }
        return str;
    }
    
    if(_name == "ranges"sv ||
       _name == "dma-ranges"sv
    ) {
        auto value = get_prop_encoded_array<struct details::fdt_range>();
        auto str = std::format("{}{}:\n", indent, _name);
        for(auto&& item : value) {
            auto child_start = details::be_to_host(item->child_bus_address);
            auto parent_start = details::be_to_host(item->parent_bus_address);
            auto len = details::be_to_host(item->length);
            auto line = std::format("{}  - 0x{:X} -> 0x{:X} ({} bytes)\n", indent, child_start, parent_start, len);
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

template<>
devicetree::details::iterator<devicetree::property>& devicetree::details::iterator<devicetree::property>::operator++() {
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