#include "fdt.hpp"

std::string devicetree::fdt::format() const {
    return std::format("Devicetree: 0x{:X}", _start);
}
