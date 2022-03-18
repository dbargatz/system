#include "fdt.hpp"

std::string devicetree::fdt::format() const {
    return std::format("Devicetree: 0x{:X}", (const void *)_header);
}
