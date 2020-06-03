#include "idt.hpp"
#include "inline_asm.hpp"

struct IDT::idt IDT::our_idt;

// TODO: Handling logging this way is gross. Fix logging once migration complete.
#include "../../../kernel/platform/qemu-system-x86_64/boot/logger.hpp"
extern kernel::platform::x86_64::logger gLog;

void IDT::init() {
    // First, disable interrupts before we go trashing the IDT.
    disable_interrupts();

    // TODO: Likely a better way to do this with a memset implementation
    // Clear out the entire IDT.
    for(uint16_t idx = 0; idx < sizeof(our_idt); idx++) {
        ((uint8_t *)&our_idt)[idx] = 0;
    }

    struct idtr idt_info = { .limit = sizeof(our_idt), .offset = &our_idt };
    asm volatile("lidt %0": :"m"(idt_info));
    gLog.debug("IDT loaded: address {#016X}, length {}.\n", (uint64_t)idt_info.offset, idt_info.limit);
}

void IDT::disable_interrupts() {
    cli();
    gLog.info("Interrupts disabled.\n");
}

void IDT::enable_interrupts() {
    sti();
    gLog.info("Interrupts enabled.\n");
}

bool IDT::register_handler(uint8_t in_index, void (*in_handler)(struct interrupt_frame *)) {
    // TODO: ensure that this handler isn't already registered
    our_idt.descriptors[in_index].offset_0_15  = (uint16_t)(((uint64_t)in_handler >>  0) & 0x0000FFFF);
    our_idt.descriptors[in_index].offset_16_31 = (uint16_t)(((uint64_t)in_handler >> 16) & 0x0000FFFF);
    our_idt.descriptors[in_index].offset_32_63 = (uint32_t)(((uint64_t)in_handler >> 32) & 0xFFFFFFFF);
    //                                               Ring 0    GDT             Ring 0 code segment
    our_idt.descriptors[in_index].seg_selector = { .rpl = 0, .table_idx = 0, .descriptor_idx = 1};
    //                                       Interrupt Gate     System Segment
    our_idt.descriptors[in_index].type = { .gate_type = 0xE, .segment_type = 0, .dpl = 0, .present = 1 };
    gLog.debug("Registered handler {#016X} for index {#02X} ({}).\n", (uint64_t)in_handler, in_index, in_index);

    return true;
}
