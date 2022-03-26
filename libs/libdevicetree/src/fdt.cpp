#include "fdt.hpp"
#include <cassert>
#include "__utils.hpp"

devicetree::fdt devicetree::fdt::parse(const void * in_ptr) {
    auto header = (struct internal::fdt_header *)in_ptr;
    assert(internal::be_to_le(header->magic) == 0xD00DFEED);
    assert(internal::be_to_le(header->version) == 17);

    auto byte_ptr = (const std::uint8_t *)in_ptr;
    auto memrsv_ptr = byte_ptr + internal::be_to_le(header->off_mem_rsvmap);
    auto structs_block_ptr = byte_ptr + internal::be_to_le(header->off_dt_struct);
    auto structs_block_size = internal::be_to_le(header->size_dt_struct);
    auto strings_block_ptr = byte_ptr + internal::be_to_le(header->off_dt_strings);
    auto fdt = devicetree::fdt();
    fdt._header = header;
    fdt._mem_reserve_map = (struct internal::fdt_memory_reserve_entry *)memrsv_ptr;
    fdt._root = node::parse(structs_block_ptr, strings_block_ptr);

    assert((fdt._root->length() + sizeof(std::uint32_t)) == structs_block_size);
    auto end = structs_block_ptr + fdt._root->length();
    auto end_token = internal::be_to_le(*(std::uint32_t *)end);
    assert(end_token == 0x09);

    return fdt;
}

devicetree::node * devicetree::fdt::find(std::string_view in_name) {
    assert(in_name.starts_with('/'));
    return _root->find(in_name);
}

std::string devicetree::fdt::format() const {
    auto indent = std::string(2, ' ');
    auto lenstr = std::format("{}Length: {}", indent, internal::be_to_le(_header->totalsize));
    auto cpustr = std::format("{}Boot CPU ID: {}", indent, internal::be_to_le(_header->boot_cpuid_phys));
    auto memrsv_str = std::format("{}Reserved Memory:\n", indent);
    indent.append(2, ' ');

    auto entry = _mem_reserve_map;
    while(true) {
        auto addr = internal::be_to_le(entry->address);
        auto size = internal::be_to_le(entry->size);
        if(addr == 0 && size == 0) { break; }

        auto end_addr = addr + size - 1;
        auto entry_str = std::format("{} 0x{:X}-0x{:X} [{} bytes]\n", indent, addr, end_addr, size);
        memrsv_str.append(entry_str);
        entry++;
    }

    auto hdr_str = std::format("{}\n{}\n{}", lenstr, cpustr, memrsv_str);
    return std::format("Devicetree: 0x{:X}\n{}{}", (const void *)_header, hdr_str, _root->format(1));
}

std::size_t devicetree::fdt::length() const {
    return internal::be_to_le(_header->totalsize);
}
