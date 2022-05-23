#ifndef _DEVICETREE_PROPERTIES_REG_HPP
#define _DEVICETREE_PROPERTIES_REG_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include "../__utils.hpp"
#include "../property.hpp"

namespace devicetree::properties {

struct reg_entry {
    std::uint64_t base;
    std::size_t length;
}; // struct reg_entry

class reg_iterator {
private:
    std::size_t _addr_cells;
    std::size_t _size_cells;
    std::uint8_t * _current;

    std::uint64_t _get_cells(std::uint32_t in_num_cells, std::uint32_t in_offset) {
        // Currently, we can only return the value for one cell (32-bit) or two cells (64-bit).
        assert(in_num_cells == 1 || in_num_cells == 2);

        // Make sure the value offset is aligned on a number-of-cells boundary.
        auto cell_size = sizeof(std::uint32_t) * in_num_cells;
        assert((in_offset % cell_size) == 0);
        auto ptr = _current + in_offset;
        switch(in_num_cells) {
            case 1:  { return details::be_to_host(*(std::uint32_t *)ptr); }
            case 2:  { return details::be_to_host(*(std::uint64_t *)ptr); }
            default: { assertm(false, "unreachable - in_num_cells value unsupported!"); }
        }
    }

public:
    reg_iterator(std::uint8_t * in_ptr, std::uint32_t in_addr_cells, std::uint32_t in_size_cells) {
        _addr_cells = in_addr_cells;
        _size_cells = in_size_cells;
        _current = in_ptr;
    }

    bool operator==(const reg_iterator& in_other) const { return (_current == in_other._current); }
    bool operator!=(const reg_iterator& in_other) const { return !(*this == in_other); }

    struct reg_entry operator*() {
        auto base = _get_cells(_addr_cells, 0);
        auto length = _get_cells(_size_cells, sizeof(std::uint32_t) * _addr_cells);
        return reg_entry { base, length };
    }

    reg_iterator& operator++() {
        if(_current == nullptr) { return *this; }
        _current += sizeof(std::uint32_t) * (_addr_cells + _size_cells);
        return *this;
    }
}; // class reg_iterator

class reg : public devicetree::property {
public:
    reg(property& in_prop, std::uint32_t in_parent_addr_cells, std::uint32_t in_parent_size_cells, std::uint32_t in_child_addr_cells, std::uint32_t in_child_size_cells) : property(in_prop) {
        _addr_cells = in_parent_addr_cells;
        _size_cells = in_parent_size_cells;
    }

    reg(std::uint8_t * in_first, std::uint32_t in_parent_addr_cells, std::uint32_t in_parent_size_cells, std::uint32_t in_child_addr_cells, std::uint32_t in_child_size_cells) : property(in_first) { 
        _addr_cells = in_parent_addr_cells;
        _size_cells = in_parent_size_cells;
    }

    reg_iterator begin() noexcept { return reg_iterator((std::uint8_t *)_start->value, _addr_cells, _size_cells); }
    reg_iterator end() noexcept { return reg_iterator((std::uint8_t *)_start->value + value_length(), _addr_cells, _size_cells); }

protected:
    std::uint32_t _addr_cells;
    std::uint32_t _size_cells;
}; // class reg

}; // namespace devicetree::properties

#endif // _DEVICETREE_PROPERTIES_REG_HPP