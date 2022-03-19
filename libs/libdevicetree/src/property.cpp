#include "property.hpp"
#include <cassert>
#include "__utils.hpp"

using namespace std::literals;

devicetree::property * devicetree::property::parse(const void * in_ptr, const void * in_strings_block) {
    auto p = new property();
    p->next = nullptr;
    p->_start = (struct internal::fdt_property *)in_ptr;
    assert(internal::be_to_le(p->_start->token) == 0x00000003);

    auto name_ptr = (const char *)in_strings_block + internal::be_to_le(p->_start->nameoff);
    p->_name = std::string_view(name_ptr);
    auto len = sizeof(*p->_start) + internal::be_to_le(p->_start->len);
    p->_length = internal::align(len);

    return p;
}

template<> std::uint32_t devicetree::property::get_value<std::uint32_t>() const {
    assert(internal::be_to_le(_start->len) == sizeof(std::uint32_t));
    return internal::be_to_le(*(std::uint32_t *)_start->value);
}

template<> std::uint64_t devicetree::property::get_value<std::uint64_t>() const {
    assert(internal::be_to_le(_start->len) == sizeof(std::uint64_t));
    return internal::be_to_le(*(std::uint64_t *)_start->value);
}

template<> std::string_view devicetree::property::get_value<std::string_view>() const {
    return std::string_view((const char *)_start->value);
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
    } else if(
        _name == "phandle"sv ||
        _name == "#address-cells"sv ||
        _name == "#size-cells"sv ||
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
    } else if(
        _name == "cpu-release-addr"sv
    ) {
        auto value = get_value<std::uint64_t>();
        return std::format("{}{}: {}\n", indent, _name, value);
    }
    return std::format("{}{}: {}\n", indent, _name, "???"sv);
}

std::size_t devicetree::property::length() const {
    return _length;
}