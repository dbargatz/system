#ifndef _INTERRUPTS_IDT_HPP
#define _INTERRUPTS_IDT_HPP

#include <cstdint>
#include "../../../../logging/logger.hpp"

class idt {
private:
    struct segment_selector {
        std::uint16_t rpl: 2;
        std::uint16_t table_idx: 1;
        std::uint16_t descriptor_idx: 13;
    } __attribute__((packed));

    struct descriptor_attrs {
        std::uint8_t gate_type: 4;
        std::uint8_t segment_type: 1;
        std::uint8_t dpl: 2;
        std::uint8_t present: 1;
    } __attribute__((packed));

    struct idt_descriptor {
        std::uint16_t offset_0_15;
        struct segment_selector seg_selector;
        std::uint8_t  ist_index;
        struct descriptor_attrs type;
        std::uint16_t offset_16_31;
        std::uint32_t offset_32_63;
        std::uint32_t reserved;
    } __attribute__((packed));

    logging::logger& _log;
    struct idt_descriptor _idt[256];

public:
    /**
     * @brief Construct a new Interrupt Descriptor Table (IDT) object.
     * 
     * @param in_log logger for member functions
     */
    idt(logging::logger& in_log);

    /**
     * @brief Inserts the given handler at the given index in the IDT, such
     * that when the interrupt/exception of that index (vector) occurs, the
     * handler is invoked.
     * 
     * @param in_index the IDT index to insert the handler at
     * @param in_handler function to execute when the interrupt occurs
     * @param in_ist_index Interrupt Stack Table (IST) stack to handle interrupt
     *                     on; index into TSS
     * @param in_ring0 if true, handler only accessible from ring 0; otherwise,
     *                 accessible only from ring 3
     */
    void register_handler(std::uint8_t in_index, const void * in_handler,
                          std::uint8_t in_ist_index = 0, bool in_ring0 = true);

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
