#ifndef _INTERRUPTS_IDT_HPP
#define _INTERRUPTS_IDT_HPP

#include "../std/stdint.h"

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
        uint16_t descriptor_idx: 13;
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

    struct idt_descriptor idt[256];

public:
    /**
     * @brief Construct a new IDT object.
     */
    IDT();

    /**
     * @brief Inserts the given handler at the given index in the IDT, such
     * that when the interrupt/exception of that index (vector) occurs, the
     * handler is invoked.
     * 
     * @param in_index the IDT index to insert the handler at
     * @param in_handler function to execute when the interrupt occurs
     */
    void register_handler(uint8_t in_index, void (*in_handler)(struct interrupt_frame *));

    /**
     * @brief Loads this IDT into the core such that interrupts/exceptions will
     * be handled by this IDT.
     * 
     * Uses the lidt instruction to load the new IDT. @see Intel SDM, Volume 3A,
     * Section 6.10 for more information.
     */
    void install();
};

#endif // _INTERRUPTS_IDT_HPP
