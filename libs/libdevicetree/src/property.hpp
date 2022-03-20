#ifndef _DEVICETREE_PROPERTY_HPP
#define _DEVICETREE_PROPERTY_HPP

#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include <vector>
#include "__structs.hpp"
#include "__utils.hpp"

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

    template<class V> V get_value() const;
    template<class V> std::vector<V*> get_prop_encoded_array() const {
        auto vec = std::vector<V*>();
        auto cur_ptr = (const std::uint8_t *)_start->value;
        auto end_ptr = (const std::uint8_t *)_start->value + internal::be_to_le(_start->len);
        while(cur_ptr < end_ptr) {
            auto value = (V*)cur_ptr;
            vec.push_back(value);
            cur_ptr += sizeof(V);
        }
        return vec;
    }

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