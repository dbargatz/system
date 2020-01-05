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

    SAVE_REG(state, rax);
    SAVE_REG(state, rbx);
    SAVE_REG(state, rcx);
    SAVE_REG(state, rdx);
    SAVE_REG(state, rsi);
    SAVE_REG(state, rdi);
    SAVE_REG(state, r8);
    SAVE_REG(state, r9);
    SAVE_REG(state, r10);
    SAVE_REG(state, r11);
    SAVE_REG(state, r12);
    SAVE_REG(state, r13);
    SAVE_REG(state, r14);
    SAVE_REG(state, r15);

    SAVE_REG(state, ds);
    SAVE_REG(state, es);
    SAVE_REG(state, fs);
    SAVE_REG(state, gs);

    SAVE_REG(state, cr0);
    SAVE_REG(state, cr2);
    SAVE_REG(state, cr3);
    SAVE_REG(state, cr4);

    SAVE_REG(state, dr0);
    SAVE_REG(state, dr1);
    SAVE_REG(state, dr2);
    SAVE_REG(state, dr3);
    SAVE_REG(state, dr6);
    SAVE_REG(state, dr7);

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
    screen.write(text("\trip:    {#016X}\n", state.rip).get(), fg, bg);
    // TODO: get correct RBP
    screen.write(text("\trsp:    {#016X}\trbp:    {#016X}\n", state.rsp, state.rbp).get(), fg, bg);

    // Dump the flags register.
    screen.write(text("\n\trflags: {#016X}\n", state.rflags).get(), fg, bg);

    screen.write(text("\trax:    {#016X}\trbx:    {#016X}\n", state.rax, state.rbx).get(), fg, bg);
    screen.write(text("\trcx:    {#016X}\trdx:    {#016X}\n", state.rcx, state.rdx).get(), fg, bg);
    screen.write(text("\trsi:    {#016X}\trdi:    {#016X}\n", state.rsi, state.rdi).get(), fg, bg);
    screen.write(text("\tr8:     {#016X}\tr9:     {#016X}\n", state.r8, state.r9).get(), fg, bg);
    screen.write(text("\tr10:    {#016X}\tr11:    {#016X}\n", state.r10, state.r11).get(), fg, bg);
    screen.write(text("\tr12:    {#016X}\tr13:    {#016X}\n", state.r12, state.r13).get(), fg, bg);
    screen.write(text("\tr14:    {#016X}\tr15:    {#016X}\n", state.r14, state.r15).get(), fg, bg);
    
    // Dump selectors.
    screen.write(text("\n\tcs:     {#016X}\tds:     {#016X}\n", state.cs, state.ds).get(), fg, bg);
    screen.write(text("\tes:     {#016X}\tfs:     {#016X}\n", state.es, state.fs).get(), fg, bg);
    screen.write(text("\tgs:     {#016X}\tss:     {#016X}\n", state.gs, state.ss).get(), fg, bg);
    
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