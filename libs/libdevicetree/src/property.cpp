#include "property.hpp"
#include <cassert>
#include "__utils.hpp"

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

std::string devicetree::property::format(std::size_t in_indent) const {
    auto indent = std::string(in_indent * 2, ' ');
    return std::format("{}{}: ???\n", indent, _name);
}

std::size_t devicetree::property::length() const {
    return _length;
}