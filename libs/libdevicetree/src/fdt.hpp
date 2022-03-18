#ifndef _DEVICETREE_FDT_HPP
#define _DEVICETREE_FDT_HPP

#include <cassert>
#include <format>
#include <string>
#include "__structs.hpp"
#include "__utils.hpp"

namespace devicetree {

class fdt {
private:
    struct internal::fdt_header * _header;
    struct internal::fdt_begin_node * _root;

public:
    fdt(const void * in_ptr) {
        _header = (struct internal::fdt_header *)in_ptr;
        assert(_header->magic == internal::be_to_le(0xD00DFEED));
        assert(_header->version == internal::be_to_le((std::uint32_t)17));
    }

    std::string format() const;
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