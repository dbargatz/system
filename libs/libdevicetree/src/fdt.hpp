#ifndef _DEVICETREE_FDT_HPP
#define _DEVICETREE_FDT_HPP

#include <format>
#include <string>
#include <string_view>
#include "__structs.hpp"
#include "export.hpp"
#include "node.hpp"

namespace devicetree {

class fdt {
private:
    const struct internal::fdt_header * _header;
    const struct internal::fdt_memory_reserve_entry * _mem_reserve_map;
    const std::uint8_t * _strings_block_ptr;
    const std::uint8_t * _structs_block_ptr;
    std::size_t _structs_block_size;

public:
    fdt() = delete;
    fdt(const void * in_ptr);

    bool find(std::string_view in_name, node * out_node);

    std::string format() const;
    std::size_t length() const;
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