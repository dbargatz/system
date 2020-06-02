#include "platform/qemu-system-x86_64/boot/logger.hpp"
#include "platform/qemu-system-x86_64/boot/panic.h"
#include "platform/qemu-system-x86_64/boot/vga.hpp"
#include "platform/qemu-system-x86_64/types/text.hpp"

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

extern "C" int kmain(const void * in_boot_info) {
    using kernel::platform::x86_64::logger;
    using kernel::platform::x86_64::types::text;

    logger log;

    log.debug("Default format string: {}, {}, \"{}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 1");
    log.info("With format specifiers: {x}, {X}, \"{s}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 2");
    log.warn("With prefixes: {#x}, {#X}, \"{#s}\"\n", (uint64_t)42, (uint64_t)63, (const char *)"raw string 3");
    log.error("Here is some error text\n");

    // HACK: Set up the IDT so we can use the UD2 instruction for panic(), which
    //       causes a processor exception. This allows the exception handler to
    //       dump registers unmodified, shown as they were when the panic 
    //       occurred. 
    for(int idx = 0; idx < sizeof(gIdt); idx++) {
        ((char *)&gIdt)[idx] = 0;
    }

    gIdt.descriptors[6].offset_0_15  = (uint16_t)(((uint64_t)panic_handler >>  0) & 0x0000FFFF);
    gIdt.descriptors[6].offset_16_31 = (uint16_t)(((uint64_t)panic_handler >> 16) & 0x0000FFFF);
    gIdt.descriptors[6].offset_32_63 = (uint32_t)(((uint64_t)panic_handler >> 32) & 0xFFFFFFFF);
    //                                   Ring 0    GDT             Ring 0 code segment
    gIdt.descriptors[6].seg_selector = { .rpl = 0, .table_idx = 0, .descriptor_idx = 1};
    //                           Interrupt Gate     System Segment
    gIdt.descriptors[6].type = { .gate_type = 0xE, .segment_type = 0, .dpl = 0, .present = 1 }; 

    struct idtr idt_info;
    idt_info.limit = sizeof(struct idt);
    idt_info.offset = &gIdt;
    asm volatile("lidt %0": :"m"(idt_info));

    log.info("Delaying for a few seconds before panic...\n");
    delay(300);

    PANIC("End of kmain reached!");
    return -1;
}