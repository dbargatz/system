#ifndef _IDT_HPP
#define _IDT_HPP

// TODO: redirect to local stdint.h when migration complete.
#include "../../../kernel/platform/qemu-system-x86_64/types/stdint.h"

struct interrupt_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));

class IDT {
private:
    struct segment_selector {
        uint16_t rpl: 2;
        uint16_t table_idx: 1;
        uint16_t descriptor_idx: 12;
    } __attribute__((packed));

    struct descriptor_attrs {
        uint8_t gate_type: 4;
        uint8_t segment_type: 1;
        uint8_t dpl: 2;
        uint8_t present: 1;
    } __attribute__((packed));

    struct idt_descriptor {
        uint16_t offset_0_15;
        struct segment_selector seg_selector;
        uint8_t  ist_index;
        struct descriptor_attrs type;
        uint16_t offset_16_31;
        uint32_t offset_32_63;
        uint32_t reserved;
    } __attribute__((packed));

    struct idt {
        struct idt_descriptor descriptors[256];
    } __attribute__((packed));

    struct idtr {
        uint16_t limit;
        void *   offset;
    } __attribute__((packed));

    static struct idt our_idt;

public:
    static void init();
    static void disable_interrupts();
    static void enable_interrupts();
    static bool register_handler(uint8_t in_index, void (*in_handler)(struct interrupt_frame *));
};

#endif // _IDT_HPP
