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

extern "C" __attribute__((interrupt))
void panic_handler(struct interrupt_frame * in_frame) {
    struct register_state state = { 
        .rip = in_frame->rip, 
        .rflags = in_frame->rflags,
        .rsp = in_frame->rsp,

        .cs = in_frame->cs, 
        .ss = in_frame->ss,

        .cr0 = 0,
        .cr2 = 0,
        .cr3 = 0,
        .cr4 = 0,

        .dr0 = 0,
        .dr1 = 0,
        .dr2 = 0,
        .dr3 = 0,
        .dr6 = 0,
        .dr7 = 0,
    };

    asm volatile("movq %%rax, %0" : "=r"(state.rax));
    asm volatile("movq %%rbx, %0" : "=r"(state.rbx));
    asm volatile("movq %%rcx, %0" : "=r"(state.rcx));
    asm volatile("movq %%rdx, %0" : "=r"(state.rdx));
    asm volatile("movq %%rsi, %0" : "=r"(state.rsi));
    asm volatile("movq %%rdi, %0" : "=r"(state.rdi));
    asm volatile("movq %%r8, %0"  : "=r"(state.r8));
    asm volatile("movq %%r9, %0"  : "=r"(state.r9));
    asm volatile("movq %%r10, %0" : "=r"(state.r10));
    asm volatile("movq %%r11, %0" : "=r"(state.r11));
    asm volatile("movq %%r12, %0" : "=r"(state.r12));
    asm volatile("movq %%r13, %0" : "=r"(state.r13));
    asm volatile("movq %%r14, %0" : "=r"(state.r14));
    asm volatile("movq %%r15, %0" : "=r"(state.r15));
    asm volatile("movq %%ds, %0"  : "=r"(state.ds));
    asm volatile("movq %%es, %0"  : "=r"(state.es));
    asm volatile("movq %%fs, %0"  : "=r"(state.fs));
    asm volatile("movq %%gs, %0"  : "=r"(state.gs));

    using kernel::platform::x86_64::vga;
    using kernel::platform::x86_64::types::text;

    vga screen;
    vga::color fg = vga::color::white;
    vga::color bg = vga::color::bright_magenta;
    screen.clear_screen(bg);
    screen.set_position(0, 0);
    // TODO: print custom panic message?
    screen.write("PANIC\n", fg, bg);

    // Dump RIP and the stack pointers.
    screen.write(text("\trip:    {#X}\n", state.rip).get(), fg, bg);
    screen.write(text("\trsp:    {#X}\trbp:    {#X}\n", state.rsp, state.rbp).get(), fg, bg);

    // Dump the flags register.
    screen.write(text("\trflags: {#X}\n", state.rflags).get(), fg, bg);

    screen.write(text("\trax:    {#X}\trbx:    {#X}\n", state.rax, state.rbx).get(), fg, bg);
    screen.write(text("\trcx:    {#X}\trdx:    {#X}\n", state.rcx, state.rdx).get(), fg, bg);
    screen.write(text("\trsi:    {#X}\trdi:    {#X}\n", state.rsi, state.rdi).get(), fg, bg);
    screen.write(text("\tr8:     {#X}\tr9:     {#X}\n", state.r8, state.r9).get(), fg, bg);
    screen.write(text("\tr10:    {#X}\tr11:    {#X}\n", state.r10, state.r11).get(), fg, bg);
    screen.write(text("\tr12:    {#X}\tr13:    {#X}\n", state.r12, state.r13).get(), fg, bg);
    screen.write(text("\tr14:    {#X}\tr15:    {#X}\n", state.r14, state.r15).get(), fg, bg);
    
    // Dump selectors.
    screen.write(text("\tcs:     {#X}\tds:     {#X}\n", state.cs, state.ds).get(), fg, bg);
    screen.write(text("\tes:     {#X}\tfs:     {#X}\n", state.es, state.fs).get(), fg, bg);
    screen.write(text("\tgs:     {#X}\tss:     {#X}\n", state.gs, state.ss).get(), fg, bg);
    
    // Dump the CR registers.
    // TODO: dump CR0
    // TODO: dump CR2
    // TODO: dump CR3
    // TODO: dump CR4

    // Dump the debug registers.
    // TODO: dump DR0
    // TODO: dump DR1
    // TODO: dump DR2
    // TODO: dump DR3
    // TODO: dump DR4
    // TODO: dump DR5
    // TODO: dump DR6
    // TODO: dump DR7

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

    unsigned long long x = 200000000;
    while(--x > 0) {
        // Loop for a bit
        if(0 == x % 25000000) {
            log.debug("Panic in {}\n", x / 25000000);
        }
    }

    panic();
    return -1;
}