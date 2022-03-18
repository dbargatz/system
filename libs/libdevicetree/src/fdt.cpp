#include "fdt.hpp"
#include <cassert>
#include "__utils.hpp"

devicetree::fdt * devicetree::fdt::parse(const void * in_ptr) {
    auto header = (struct internal::fdt_header *)in_ptr;
    assert(internal::be_to_le(header->magic) == 0xD00DFEED);
    assert(internal::be_to_le(header->version) == 17);

    auto byte_ptr = (const std::uint8_t *)in_ptr;
    auto structs_block_ptr = byte_ptr + internal::be_to_le(header->off_dt_struct);
    auto structs_block_size = internal::be_to_le(header->size_dt_struct);
    auto fdt = new devicetree::fdt();
    fdt->_header = header;
    fdt->_root = node::parse(structs_block_ptr);

    assert((fdt->_root->length() + sizeof(std::uint32_t)) == structs_block_size);
    auto end = structs_block_ptr + fdt->_root->length();
    auto end_token = internal::be_to_le(*(std::uint32_t *)end);
    assert(end_token == 0x09);

    return fdt;
}

std::string devicetree::fdt::format() const {
    return std::format("Devicetree: 0x{:X}\n  {}\n", (const void *)_header, *_root);
}

std::size_t devicetree::fdt::length() const {
    return internal::be_to_le(_header->totalsize);
}
