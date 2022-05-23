#ifndef _DEVICETREE_PROPERTIES_RANGES_HPP
#define _DEVICETREE_PROPERTIES_RANGES_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include "../__utils.hpp"
#include "../property.hpp"

namespace devicetree::properties {

struct ranges_entry {
    std::uint64_t child_bus_address;
    std::uint64_t parent_bus_address;
    std::size_t length;
}; // struct ranges_entry

class ranges_iterator {
private:
    std::size_t _parent_addr_cells;
    std::size_t _child_addr_cells;
    std::size_t _child_size_cells;
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
    ranges_iterator(std::uint8_t * in_ptr, std::uint32_t in_parent_addr_cells, std::uint32_t in_child_addr_cells, std::uint32_t in_child_size_cells) {
        _parent_addr_cells = in_parent_addr_cells;
        _child_addr_cells = in_child_addr_cells;
        _child_size_cells = in_child_size_cells;
        _current = in_ptr;
    }

    bool operator==(const ranges_iterator& in_other) const { return (_current == in_other._current); }
    bool operator!=(const ranges_iterator& in_other) const { return !(*this == in_other); }

    struct ranges_entry operator*() {
        auto offset = 0;
        auto child_addr = _get_cells(_child_addr_cells, offset);
        offset += sizeof(std::uint32_t) * _child_addr_cells;
        auto parent_addr = _get_cells(_parent_addr_cells, offset);
        offset += sizeof(std::uint32_t) * _parent_addr_cells;
        auto length = _get_cells(_child_size_cells, offset);
        return ranges_entry { child_addr, parent_addr, length };
    }

    ranges_iterator& operator++() {
        if(_current == nullptr) { return *this; }
        _current += sizeof(std::uint32_t) * (_child_addr_cells + _parent_addr_cells + _child_size_cells);
        return *this;
    }
}; // class ranges_iterator

class ranges : public devicetree::property {
public:
    ranges(property& in_prop, std::uint32_t in_parent_addr_cells, std::uint32_t in_parent_size_cells, std::uint32_t in_child_addr_cells, std::uint32_t in_child_size_cells) : property(in_prop) {
        _parent_addr_cells = in_parent_addr_cells;
        _child_addr_cells = in_child_addr_cells;
        _child_size_cells = in_child_size_cells;
    }

    ranges(std::uint8_t * in_first, std::uint32_t in_parent_addr_cells, std::uint32_t in_parent_size_cells, std::uint32_t in_child_addr_cells, std::uint32_t in_child_size_cells) : property(in_first) { 
        _parent_addr_cells = in_parent_addr_cells;
        _child_addr_cells = in_child_addr_cells;
        _child_size_cells = in_child_size_cells;
    }

    ranges_iterator begin() noexcept { return ranges_iterator((std::uint8_t *)_start->value, _parent_addr_cells, _child_addr_cells, _child_size_cells); }
    ranges_iterator end() noexcept { return ranges_iterator((std::uint8_t *)_start->value + value_length(), _parent_addr_cells, _child_addr_cells, _child_size_cells); }

protected:
    std::size_t _parent_addr_cells;
    std::size_t _child_addr_cells;
    std::size_t _child_size_cells;
}; // class ranges

}; // namespace devicetree::properties

#endif // _DEVICETREE_PROPERTIES_RANGES_HPP