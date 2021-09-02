#include "paging.hpp"
 
 cpu::x64::memory::pml4::pml4(void* in_existing_pml4) {
    if(in_existing_pml4 == nullptr) {
        in_existing_pml4 = new struct _pml4;
    }
    _pml4_val = (struct _pml4*)in_existing_pml4;
}

physaddr_t cpu::x64::memory::pml4::get_physaddr(virtaddr_t in_virtual_addr) {
    auto entry_idx = (in_virtual_addr & 0x0000FF8000000000) >> 39;
    auto entry = _pml4_val->_entries[entry_idx];
    return (physaddr_t)entry.pml3_physaddr;
}
