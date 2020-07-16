#include "idt.hpp"
#include "../std/memset.hpp"

struct idtr {
    uint16_t limit;
    void *   offset;
} __attribute__((packed));

idt::idt(logger& in_log) : _log(in_log) {
    _log.debug("Zeroing IDT...");
    // Zero out the actual Interrupt Descriptor Table, which is stored as a 
    // class member.
    memset(_idt, 0, sizeof(_idt));
    _log.debug("Zeroed.");
}

void idt::register_handler(uint8_t in_index, const void * in_handler,
                           uint8_t in_ist_index) {
    // TODO: ensure that this handler isn't already registered?
    _idt[in_index].offset_0_15  = (uint16_t)(((uint64_t)in_handler >>  0) & 0x0000FFFF);
    _idt[in_index].offset_16_31 = (uint16_t)(((uint64_t)in_handler >> 16) & 0x0000FFFF);
    _idt[in_index].offset_32_63 = (uint32_t)(((uint64_t)in_handler >> 32) & 0xFFFFFFFF);
    //                              Ring 0           IST Index           Ring 0 code segment
    _idt[in_index].seg_selector = { .rpl = 0, .table_idx = in_ist_index, .descriptor_idx = 1};
    //                      Interrupt Gate     System Segment
    _idt[in_index].type = { .gate_type = 0xE, .segment_type = 0, .dpl = 0, .present = 1 };
}

void idt::install() {
    // Populate an IDTR struct and use lidt to install the IDT.
    struct idtr idtr = {
        .limit = sizeof(_idt),
        .offset = (void *)_idt
    };
    asm volatile("lidt %0": :"m"(idtr));
    _log.debug("IDT loaded: address {#016X}, length {}.", (uint64_t)idtr.offset, idtr.limit);
}
