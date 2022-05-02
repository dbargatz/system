#ifndef _DEVICETREE_PROPERTY_HPP
#define _DEVICETREE_PROPERTY_HPP

#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include <vector>
#include "__iterator.hpp"
#include "__list.hpp"
#include "__structs.hpp"
#include "__utils.hpp"

namespace devicetree {

template <typename A, typename S>
struct reg {
    A address;
    S length;
}; // struct reg

using reg32 = reg<std::uint32_t, std::uint32_t>;

template <typename T>
class proparray {
public:
    proparray() { _first = nullptr; _size = 0; }
    proparray(const T * in_first, std::size_t in_size) { _first = in_first; _size = in_size; }

    const T * begin() noexcept { return _first; }
    const T * end() noexcept { return (const T *)((std::uint8_t *)_first + _size); }

private:
    const T * _first;
    std::size_t _size;

}; // class proparray

using reg_proparray = proparray<reg32>;

class property {
public:
    using internal_struct = struct details::fdt_property;

    property() = delete;
    property(const void * in_ptr);

    static void set_strings_block(const std::uint8_t * in_ptr) { _s_strings_block = in_ptr; }

    template<class V> V get_value() const;

    std::string format(std::size_t in_indent = 0) const;
    std::size_t length() const;
    std::string_view name() const;

private:
    static const std::uint8_t * _s_strings_block;
    property::internal_struct * _start;
    std::string_view _name;
}; // class property

using property_iterator = details::iterator<property, struct details::fdt_property>;
using property_list = details::list<property, struct details::fdt_property>;

template <>
property_iterator& property_iterator::operator++();

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