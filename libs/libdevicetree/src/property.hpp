#ifndef _DEVICETREE_PROPERTY_HPP
#define _DEVICETREE_PROPERTY_HPP

#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include "__structs.hpp"

namespace devicetree {

class property {
private:
    struct internal::fdt_property * _start;
    std::string_view _name;
    std::size_t _length;

    property() {}

public:
    property * next;

    static property * parse(const void * in_ptr, const void * in_strings_block);

    std::string format(std::size_t in_indent = 0) const;
    std::size_t length() const;
}; // class property

}; // namespace devicetree

template <>
struct std::formatter<devicetree::property> {
    formatter() { }

    void parse(const string::value_type* in_open_brace,
               const string::value_type* in_close_brace) { }

    string format(const devicetree::property& in_arg) {
        return in_arg.format();
    }
}; // struct std::formatter

#endif // _DEVICETREE_PROPERTY_HPP