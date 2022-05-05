#ifndef _DEVICETREE_FDT_HPP
#define _DEVICETREE_FDT_HPP

#include <format>
#include <string>
#include <string_view>
#include "__list.hpp"
#include "__structs.hpp"
#include "node.hpp"
#include "properties/stringlist.hpp"

namespace devicetree {

class fdt {
private:
    const struct details::fdt_header * _header;
    const struct details::fdt_memory_reserve_entry * _mem_reserve_map;
    const std::uint8_t * _strings_block_ptr;
    const std::uint8_t * _structs_block_ptr;
    std::size_t _structs_block_size;

public:
    fdt() = delete;
    fdt(const void * in_ptr);

    std::expected<node, std::uint32_t> get(const char * in_path);

    template <typename P>
    std::expected<node, std::uint32_t> find(P in_predicate) {
        auto root = this->root();
        if(!root) { return root; }

        return root->find(in_predicate);
    }

    std::string format() const;
    std::size_t length() const;
    std::expected<node, std::uint32_t> root() const;
}; // class fdt

}; // namespace devicetree

template <>
struct std::formatter<devicetree::fdt> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const devicetree::fdt& in_arg) {
        return in_arg.format();
    }
}; // struct std::formatter

#endif // _DEVICETREE_FDT_HPP