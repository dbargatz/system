#include "platform/qemu-system-x86_64/boot/logger.hpp"
#include "platform/qemu-system-x86_64/boot/panic.h"
#include "platform/qemu-system-x86_64/boot/vga.hpp"
#include "platform/qemu-system-x86_64/types/text.hpp"

kernel::platform::x86_64::logger gLog;

typedef struct segment_selector {
    uint16_t rpl: 2;
    uint16_t table_idx: 1;
    uint16_t descriptor_idx: 12;
} __attribute__((packed)) segment_selector_t;

typedef struct descriptor_attrs {
    uint8_t gate_type: 4;
    uint8_t segment_type: 1;
    uint8_t dpl: 2;
    uint8_t present: 1;
} __attribute__((packed)) descriptor_attrs_t;

typedef struct idtDescriptor 
{
    uint16_t offset_0_15;
    segment_selector_t seg_selector;
    uint8_t  ist_index;
    descriptor_attrs_t type;
    uint16_t offset_16_31;
    uint32_t offset_32_63;
    uint32_t reserved;
} __attribute__((packed)) idtDescriptor_t;

struct idt
{
    idtDescriptor_t descriptors[256];
} __attribute__((packed)) gIdt;

struct idtr 
{
    uint16_t limit;
    void *   offset;
} __attribute__((packed));

struct interrupt_frame
{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));

#define SAVE_REG(state, reg) do { asm volatile("movq %%" #reg ", %0" : "=r"(state.reg)); } while(0)
struct register_state
{
    uint64_t rip;
    uint64_t rflags;

    uint64_t rbp;
    uint64_t rsp;

    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    uint64_t cs;
    uint64_t ds;
    uint64_t es;
    uint64_t fs;
    uint64_t gs;
    uint64_t ss;

    uint64_t cr0;
    uint64_t cr2;
    uint64_t cr3;
    uint64_t cr4;

    uint64_t dr0;
    uint64_t dr1;
    uint64_t dr2;
    uint64_t dr3;
    uint64_t dr6;
    uint64_t dr7;
} __attribute__((packed));

using kernel::platform::x86_64::vga;
using kernel::platform::x86_64::types::text;

void hexdump(vga& screen, const void * in_ptr, uint8_t in_count) {
    uint8_t * ptr = (uint8_t *)in_ptr;
    vga::color fg = vga::color::white;
    vga::color bg = vga::color::bright_magenta;

    for(int i = 0; i < in_count; i++) {
        if(i % 16 == 0) {
            if(i > 0) {
                screen.write("\n", fg, bg);
            }
            screen.write(text("{#016X}:\t",(uint64_t)(ptr+i)).get(), fg, bg);
        }
        screen.write(text("{02X} ", *(ptr+i)).get(), fg, bg);
    }
    screen.write("\n", fg, bg);
}

/**
 * @brief Loops the given number of times (in millions of loops) as a rough
 * delay mechanism.
 * 
 * @param in_megaloops millions of loops to perform; if given value is 25, then
 * 25,000,000 loops will be performed
 */
void delay(uint16_t in_megaloops) {
    uint64_t loops = in_megaloops * 1000000;
    while(loops-- > 0) {
        // Loop for a bit
    }
}

/**
 * @brief Write the given byte to the given I/O port address.
 * 
 * @param in_io_port I/O port to write to
 * @param in_byte Byte to write
 */
static inline void outb(const uint16_t in_io_port, const uint8_t in_byte) {
    asm volatile("outb %0, %1" : : "a"(in_byte), "Nd"(in_io_port));
}

/**
 * @brief Read a byte from the given I/O port address.
 * 
 * @param in_io_port I/O port to read from
 * @return uint8_t Byte that was read
 */
static inline uint8_t inb(const uint16_t in_io_port) {
    uint8_t retval;
    asm volatile("inb %1, %0" : "=a"(retval) : "Nd"(in_io_port));
    return retval;
}

static inline void enable_interrupts(void) {
    asm volatile("sti");
}

class PIC {
private:
    static const uint16_t PIC1_COMMAND_PORT = 0x0020;
    static const uint16_t PIC1_DATA_PORT    = 0x0021;
    static const uint16_t PIC2_COMMAND_PORT = 0x00A0;
    static const uint16_t PIC2_DATA_PORT    = 0x00A1;

    static const uint8_t EOI_COMMAND  = 0x20;
    static const uint8_t INIT_COMMAND = 0x11;

    static const uint8_t PIC2_IRQ_NUMBER = 0x02;

    static const uint8_t CPU_MODE_8086 = 0x01;

public:
    static void send_eoi(const uint8_t in_irq_number) {
        // TODO: assert 0 <= in_irq_number < 15

        if(in_irq_number >= 8) {
            outb(PIC2_COMMAND_PORT, EOI_COMMAND);
            gLog.debug("Sent EOI to PIC2 for IRQ {}\n", in_irq_number);
        }
        outb(PIC1_COMMAND_PORT, EOI_COMMAND);
        gLog.debug("Sent EOI to PIC1 for IRQ {}\n", in_irq_number);
    }

    static void remap(const uint8_t in_pic1_interrupt_base, 
                      const uint8_t in_pic2_interrupt_base) {

        // Save off the current interrupt masks.
        uint8_t pic1_mask = inb(PIC1_DATA_PORT);
        uint8_t pic2_mask = inb(PIC2_DATA_PORT);
        gLog.debug("Original masks: PIC1({#02X}) PIC2({#02X})\n", inb(PIC1_DATA_PORT), inb(PIC2_DATA_PORT));

        // Start the initialization of both PICs. After this command is sent,
        // each PIC will expect three data bytes in sequence on their data port:
        //     1. ICW2: The base interrupt vector to deliver IRQs on for this PIC.
        //     2. ICW3: The cascaded identity of the cascaded PICs (if any)
        //     3. ICW4: The CPU mode to deliver interrupts as
        // See Figures 14.75 - 14.79 in the 8259 PIC datasheet for more.
        outb(PIC1_COMMAND_PORT, INIT_COMMAND);
        outb(PIC2_COMMAND_PORT, INIT_COMMAND);

        // Send ICW2, the interrupt vector base.
        outb(PIC1_DATA_PORT, in_pic1_interrupt_base);
        outb(PIC2_DATA_PORT, in_pic2_interrupt_base);
        gLog.debug("Set interrupt bases: PIC1({#02X}) PIC2({#02X})\n", in_pic1_interrupt_base, in_pic2_interrupt_base);

        // Send ICW3, the cascaded PIC identity.
        outb(PIC1_DATA_PORT, PIC2_IRQ_NUMBER << 2);
        outb(PIC2_DATA_PORT, PIC2_IRQ_NUMBER);

        // Send ICW4, and set the delivery mode as 8086/8088.
        outb(PIC1_DATA_PORT, CPU_MODE_8086);
        outb(PIC2_DATA_PORT, CPU_MODE_8086);

        // Restore the saved interrupt masks.
        outb(PIC1_DATA_PORT, pic1_mask);
        outb(PIC2_DATA_PORT, pic2_mask);
        gLog.debug("Current masks: PIC1({#02X}) PIC2({#02X})\n", inb(PIC1_DATA_PORT), inb(PIC2_DATA_PORT));
    }

    static void disable_all(void) {
        // Mask all interrupts on both the PICs.
        outb(PIC2_DATA_PORT, 0xFF);
        outb(PIC1_DATA_PORT, 0xFF);
    }

    static void disable_irq(const uint8_t in_irq_number) {
        // TODO: assert 0 <= in_irq_number < 15
        uint8_t mask;

        if(in_irq_number < 8) {
            mask = inb(PIC1_DATA_PORT) | (1 << in_irq_number);
            outb(PIC1_DATA_PORT, mask);
        } else {
            mask = inb(PIC2_DATA_PORT) | (1 << (in_irq_number - 8));
            outb(PIC2_DATA_PORT, mask);
        }
        gLog.debug("Disabled IRQ {#02X}\n", in_irq_number);
        gLog.debug("Current masks: PIC1({#02X}) PIC2({#02X})\n", inb(PIC1_DATA_PORT), inb(PIC2_DATA_PORT));
    }

    static void enable_irq(const uint8_t in_irq_number) {
        // TODO: assert 0 <= in_irq_number < 15
        uint8_t mask;

        if(in_irq_number < 8) {
            mask = inb(PIC1_DATA_PORT) & ~(1 << in_irq_number);
            outb(PIC1_DATA_PORT, mask);
        } else {
            mask = inb(PIC2_DATA_PORT) & ~(1 << (in_irq_number - 8));
            outb(PIC2_DATA_PORT, mask);
        }
        gLog.debug("Enabled IRQ {#02X}\n", in_irq_number);
        gLog.debug("Current masks: PIC1({#02X}) PIC2({#02X})\n", inb(PIC1_DATA_PORT), inb(PIC2_DATA_PORT));
    }
};

extern "C" __attribute__((interrupt))
void panic_handler(struct interrupt_frame * in_frame) {
    vga screen;
    vga::color fg = vga::color::white;
    vga::color bg = vga::color::bright_magenta;
    struct panic_data * data = (struct panic_data *)in_frame->rip;
    screen.clear_screen(bg);
    screen.set_position(0, 0);

    hexdump(screen, (void *)data, 64);
    screen.write(text("PANIC({}:{}): {}\n", data->filename, data->lineNum, data->msg).get(), fg, bg);
    while(true) {
        // Loop forever
    }
}

extern "C" __attribute__((interrupt))
void kbd_handler(struct interrupt_frame * in_frame) {
    uint8_t keystroke = inb(0x60);
    gLog.warn("Keystroke {#02X} being dropped\n", keystroke);
    PIC::send_eoi(1);
}

extern "C" __attribute__((interrupt))
void pit_handler(struct interrupt_frame * in_frame) {
    gLog.warn("PIT fired\n");
    PIC::send_eoi(0);
}

extern "C" int kmain(const void * in_boot_info) {
    vga screen;
    screen.clear_screen(vga::color::black);
    gLog.debug("Default format string: {}, {}, \"{}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 1");
    gLog.info("With format specifiers: {x}, {X}, \"{s}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 2");
    gLog.warn("With prefixes: {#x}, {#X}, \"{#s}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 3");
    gLog.error("Here is some error text\n");

    // HACK: Set up the IDT so we can use the UD2 instruction for panic(), which
    //       causes a processor exception. This allows the exception handler to
    //       dump registers unmodified, shown as they were when the panic 
    //       occurred. 
    for(int idx = 0; idx < sizeof(gIdt); idx++) {
        ((char *)&gIdt)[idx] = 0;
    }

    // UD2 (Panic)
    gIdt.descriptors[6].offset_0_15  = (uint16_t)(((uint64_t)panic_handler >>  0) & 0x0000FFFF);
    gIdt.descriptors[6].offset_16_31 = (uint16_t)(((uint64_t)panic_handler >> 16) & 0x0000FFFF);
    gIdt.descriptors[6].offset_32_63 = (uint32_t)(((uint64_t)panic_handler >> 32) & 0xFFFFFFFF);
    //                                   Ring 0    GDT             Ring 0 code segment
    gIdt.descriptors[6].seg_selector = { .rpl = 0, .table_idx = 0, .descriptor_idx = 1};
    //                           Interrupt Gate     System Segment
    gIdt.descriptors[6].type = { .gate_type = 0xE, .segment_type = 0, .dpl = 0, .present = 1 };

    // PIT
    gIdt.descriptors[32].offset_0_15  = (uint16_t)(((uint64_t)pit_handler >>  0) & 0x0000FFFF);
    gIdt.descriptors[32].offset_16_31 = (uint16_t)(((uint64_t)pit_handler >> 16) & 0x0000FFFF);
    gIdt.descriptors[32].offset_32_63 = (uint32_t)(((uint64_t)pit_handler >> 32) & 0xFFFFFFFF);
    //                                    Ring 0    GDT             Ring 0 code segment
    gIdt.descriptors[32].seg_selector = { .rpl = 0, .table_idx = 0, .descriptor_idx = 1};
    //                            Interrupt Gate     System Segment
    gIdt.descriptors[32].type = { .gate_type = 0xE, .segment_type = 0, .dpl = 0, .present = 1 };

    // Keyboard
    gIdt.descriptors[33].offset_0_15  = (uint16_t)(((uint64_t)kbd_handler >>  0) & 0x0000FFFF);
    gIdt.descriptors[33].offset_16_31 = (uint16_t)(((uint64_t)kbd_handler >> 16) & 0x0000FFFF);
    gIdt.descriptors[33].offset_32_63 = (uint32_t)(((uint64_t)kbd_handler >> 32) & 0xFFFFFFFF);
    //                                    Ring 0    GDT             Ring 0 code segment
    gIdt.descriptors[33].seg_selector = { .rpl = 0, .table_idx = 0, .descriptor_idx = 1};
    //                            Interrupt Gate     System Segment
    gIdt.descriptors[33].type = { .gate_type = 0xE, .segment_type = 0, .dpl = 0, .present = 1 };

    struct idtr idt_info;
    idt_info.limit = sizeof(struct idt);
    idt_info.offset = &gIdt;
    asm volatile("lidt %0": :"m"(idt_info));

    gLog.info("Enabling PIC...\n");
    PIC::remap(0x20, 0x28);
    PIC::disable_all();
    //PIC::enable_irq(0x0);
    PIC::enable_irq(0x1);

    gLog.info("Enabling interrupts...\n");
    enable_interrupts();

    while(true) {
        // Loop forever
    }

    PANIC("End of kmain reached!");
    return -1;
}