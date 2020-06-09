#include "idt.hpp"
#include "../inline_asm.hpp"
#include "../std/memset.hpp"

struct idtr {
    uint16_t limit;
    void *   offset;
} __attribute__((packed));

IDT::IDT() {
    gLog.debug("Zeroing IDT...\n");
    // Zero out the actual Interrupt Descriptor Table, which is stored as a 
    // class member.
    memset(&idt, 0, sizeof(idt));
    gLog.debug("Zeroed.\n");
}

void IDT::register_handler(uint8_t in_index, void (*in_handler)(struct interrupt_frame *)) {
    // TODO: ensure that this handler isn't already registered?
    idt[in_index].offset_0_15  = (uint16_t)(((uint64_t)in_handler >>  0) & 0x0000FFFF);
    idt[in_index].offset_16_31 = (uint16_t)(((uint64_t)in_handler >> 16) & 0x0000FFFF);
    idt[in_index].offset_32_63 = (uint32_t)(((uint64_t)in_handler >> 32) & 0xFFFFFFFF);
    //                              Ring 0       GDT          Ring 0 code segment
    idt[in_index].seg_selector = { .rpl = 0, .table_idx = 0, .descriptor_idx = 1};
    //                      Interrupt Gate     System Segment
    idt[in_index].type = { .gate_type = 0xE, .segment_type = 0, .dpl = 0, .present = 1 };
    gLog.debug("Registered handler {#016X} for index {#02X} ({}).\n", (uint64_t)in_handler, in_index, in_index);
}

void IDT::install(const IDT * in_table) {
    // Populate an IDTR struct and use lidt to install the IDT.
    struct idtr idtr = { 
        .limit = sizeof(in_table->idt),
        .offset = (void *)(in_table->idt)
    };
    asm volatile("lidt %0": :"m"(idtr));
    gLog.debug("IDT loaded: address {#016X}, length {}.\n", (uint64_t)idtr.offset, idtr.limit);
}
