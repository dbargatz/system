#include "fdt.hpp"
#include <cassert>
#include "__utils.hpp"
#include "node.hpp"
#include "property.hpp"


devicetree::fdt::fdt(const void * in_ptr) {
    _header = (struct details::fdt_header *)in_ptr;
    assert(details::be_to_host(_header->magic) == 0xD00DFEED);
    assert(details::be_to_host(_header->version) == 17);

    auto byte_ptr = (const std::uint8_t *)in_ptr;
    auto memrsv_ptr = byte_ptr + details::be_to_host(_header->off_mem_rsvmap);
    _mem_reserve_map = (struct details::fdt_memory_reserve_entry *)memrsv_ptr;

    _structs_block_ptr = byte_ptr + details::be_to_host(_header->off_dt_struct);
    _structs_block_size = details::be_to_host(_header->size_dt_struct);
    _strings_block_ptr = byte_ptr + details::be_to_host(_header->off_dt_strings);

    property::set_strings_block(_strings_block_ptr);

    auto root_node = node(_structs_block_ptr);
    assert((root_node.length() + sizeof(std::uint32_t)) == _structs_block_size);
    auto end = _structs_block_ptr + root_node.length();
    auto end_token = details::be_to_host(*(std::uint32_t *)end);
    assert(end_token == details::FDT_END);
}

std::expected<devicetree::node, std::uint32_t> devicetree::fdt::get(const char * in_path) {
    auto root = this->root();
    if(!root) { return root; }

    auto path = std::string_view(in_path);
    if(!path.starts_with("/")) {
        // TODO: handle alias
    } else {
        path.remove_prefix(1);
    }
    return root->get_node(path.data());
}

devicetree::details::list<const struct devicetree::details::fdt_memory_reserve_entry *, const struct devicetree::details::fdt_memory_reserve_entry> devicetree::fdt::reserved_memory() {
    return reserved_memory_list(_mem_reserve_map);
}

std::string devicetree::fdt::format() const {
    auto indent = std::string(2, ' ');
    auto lenstr = std::format("{}Length: {}", indent, details::be_to_host(_header->totalsize));
    auto cpustr = std::format("{}Boot CPU ID: {}", indent, details::be_to_host(_header->boot_cpuid_phys));
    auto memrsv_str = std::format("{}Reserved Memory:\n", indent);
    indent.append(2, ' ');

    auto entry = _mem_reserve_map;
    while(true) {
        auto addr = details::be_to_host(entry->address);
        auto size = details::be_to_host(entry->size);
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
    return details::be_to_host(_header->totalsize);
}

std::expected<devicetree::node, std::uint32_t> devicetree::fdt::root() const {
    return node(_structs_block_ptr);
}

template<>
devicetree::reserved_memory_iterator& devicetree::reserved_memory_iterator::operator++() {
    if(_current == nullptr) { return *this; }
    _current++;
    if(_current->address == 0 && _current->size == 0) {
        _current = nullptr;
    }
    return *this;
}