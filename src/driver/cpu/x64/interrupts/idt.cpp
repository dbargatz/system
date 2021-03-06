#include "idt.hpp"
#include <cstring>

struct idtr {
    std::uint16_t limit;
    void *   offset;
} __attribute__((packed));

idt::idt(logging::logger& in_log) : _log(in_log) {
    _log.debug(u8"Zeroing IDT...");
    // Zero out the actual Interrupt Descriptor Table, which is stored as a 
    // class member.
    std::memset(_idt, 0, sizeof(_idt));
    _log.debug(u8"Zeroed.");
}

void idt::register_handler(std::uint8_t in_index, const void * in_handler,
                           std::uint8_t in_ist_index, bool in_ring0) {
    std::uint8_t dpl = (in_ring0 ? 0 : 3);

    // TODO: ensure that this handler isn't already registered?
    _idt[in_index].offset_0_15  = (std::uint16_t)(((std::uint64_t)in_handler >>  0) & 0x0000FFFF);
    _idt[in_index].offset_16_31 = (std::uint16_t)(((std::uint64_t)in_handler >> 16) & 0x0000FFFF);
    _idt[in_index].offset_32_63 = (std::uint32_t)(((std::uint64_t)in_handler >> 32) & 0xFFFFFFFF);

    //                               Ring 0           IST Index           Ring 0 code segment
    _idt[in_index].seg_selector = { .rpl = 0, .table_idx = in_ist_index, .descriptor_idx = 1};
    //                      Interrupt Gate     System Segment     Descriptor Priv Level (0 for Ring 0 only, 3 for Ring 3 and Ring 0)
    _idt[in_index].type = { .gate_type = 0xE, .segment_type = 0, .dpl = dpl, .present = 1 };    
}

void idt::install() {
    // Populate an IDTR struct and use lidt to install the IDT.
    struct idtr idtr = {
        .limit = sizeof(_idt),
        .offset = (void *)_idt
    };
    asm volatile("lidt %0": :"m"(idtr));
    _log.debug(u8"IDT loaded: address {#016X}, length {}.", (std::uint64_t)idtr.offset, idtr.limit);
}
