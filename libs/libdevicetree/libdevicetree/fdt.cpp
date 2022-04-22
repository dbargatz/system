#include "fdt.hpp"
#include <cassert>
#include "__utils.hpp"
#include "property.hpp"


devicetree::fdt::fdt(const void * in_ptr) {
    _header = (struct internal::fdt_header *)in_ptr;
    assert(internal::be_to_le(_header->magic) == 0xD00DFEED);
    assert(internal::be_to_le(_header->version) == 17);

    auto byte_ptr = (const std::uint8_t *)in_ptr;
    auto memrsv_ptr = byte_ptr + internal::be_to_le(_header->off_mem_rsvmap);
    _mem_reserve_map = (struct internal::fdt_memory_reserve_entry *)memrsv_ptr;

    _structs_block_ptr = byte_ptr + internal::be_to_le(_header->off_dt_struct);
    _structs_block_size = internal::be_to_le(_header->size_dt_struct);
    _strings_block_ptr = byte_ptr + internal::be_to_le(_header->off_dt_strings);

    property::set_strings_block(_strings_block_ptr);

    auto root_node = node(_structs_block_ptr);
    assert((root_node.length() + sizeof(std::uint32_t)) == _structs_block_size);
    auto end = _structs_block_ptr + root_node.length();
    auto end_token = internal::be_to_le(*(std::uint32_t *)end);
    assert(end_token == internal::FDT_END);
}

bool devicetree::fdt::find(std::string_view in_name, devicetree::node * out_node) {
    assert(in_name.starts_with('/'));
    assert(out_node != nullptr);
    auto root_node = node(_structs_block_ptr);
    return root_node.find(in_name, out_node);
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

    auto root_node = node(_structs_block_ptr);
    auto hdr_str = std::format("{}\n{}\n{}", lenstr, cpustr, memrsv_str);
    auto tree_str = root_node.format(1);
    return std::format("Devicetree: 0x{:X}\n{}{}", (const void *)_header, hdr_str, tree_str);
}

std::size_t devicetree::fdt::length() const {
    return internal::be_to_le(_header->totalsize);
}
