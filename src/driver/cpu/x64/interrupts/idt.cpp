#include "idt.hpp"
#include "../std/memset.hpp"

struct idtr {
    uint16_t limit;
    void *   offset;
} __attribute__((packed));

IDT::IDT(logger& in_log) : _log(in_log) {
    _log.debug("Zeroing IDT...");
    // Zero out the actual Interrupt Descriptor Table, which is stored as a 
    // class member.
    memset(idt, 0, sizeof(idt));
    _log.debug("Zeroed.");
}

void IDT::register_handler(uint8_t in_index, const void * in_handler) {
    // TODO: ensure that this handler isn't already registered?

    // Use the default stack (IST 0) unless this is an invalid opcode (6, used
    // for PANIC/ASSERT).
    // TODO: make this IST setup generic rather than hardcoded
    uint8_t ist_index = 0;
    if(6 == in_index) {
        ist_index = 1;
    }
    idt[in_index].offset_0_15  = (uint16_t)(((uint64_t)in_handler >>  0) & 0x0000FFFF);
    idt[in_index].offset_16_31 = (uint16_t)(((uint64_t)in_handler >> 16) & 0x0000FFFF);
    idt[in_index].offset_32_63 = (uint32_t)(((uint64_t)in_handler >> 32) & 0xFFFFFFFF);
    //                              Ring 0      IST Index              Ring 0 code segment
    idt[in_index].seg_selector = { .rpl = 0, .table_idx = ist_index, .descriptor_idx = 1};
    //                      Interrupt Gate     System Segment
    idt[in_index].type = { .gate_type = 0xE, .segment_type = 0, .dpl = 0, .present = 1 };
}

void IDT::install() {
    // Populate an IDTR struct and use lidt to install the IDT.
    struct idtr idtr = { 
        .limit = sizeof(idt),
        .offset = (void *)idt
    };
    asm volatile("lidt %0": :"m"(idtr));
    _log.debug("IDT loaded: address {#016X}, length {}.", (uint64_t)idtr.offset, idtr.limit);
}
