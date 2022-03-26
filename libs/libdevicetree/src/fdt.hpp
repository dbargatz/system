#ifndef _DEVICETREE_FDT_HPP
#define _DEVICETREE_FDT_HPP

#include <format>
#include <string>
#include <string_view>
#include "__structs.hpp"
#include "node.hpp"

namespace devicetree {

class fdt {
private:
    struct internal::fdt_header * _header;
    struct internal::fdt_memory_reserve_entry * _mem_reserve_map;
    node * _root;

    fdt() {}

public:
    static fdt parse(const void * in_ptr);

    node * find(std::string_view in_name);

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