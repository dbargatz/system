#include "platform/qemu-system-x86_64/boot/logger.hpp"
#include "platform/qemu-system-x86_64/boot/panic.h"
#include "platform/qemu-system-x86_64/boot/vga.hpp"
#include "platform/qemu-system-x86_64/types/text.hpp"
#include "../driver/cpu/x64/interrupts/idt.hpp"
#include "../driver/cpu/x64/inline_asm.hpp"
#include "../driver/cpu/x64/interrupts/pic.hpp"

kernel::platform::x86_64::logger gLog;
using kernel::platform::x86_64::vga;
using kernel::platform::x86_64::types::text;

IDT idt;
uint64_t pit_count;

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

extern "C" __attribute__((interrupt))
void kbd_handler(struct interrupt_frame * in_frame) {
    uint8_t keystroke = inb(0x60);
    gLog.warn("Keystroke {#02X} being dropped\n", keystroke);
    PIC::send_eoi(1);
}

extern "C" __attribute__((interrupt))
void pit_handler(struct interrupt_frame * in_frame) {
    if(pit_count > 0 && 0 == (pit_count % 100)) {
        gLog.info("PIT has fired {} times.\n", pit_count);
    }
    pit_count++;
    PIC::send_eoi(0);
}

extern "C" int kmain(const void * in_boot_info) {
    vga screen;
    screen.clear_screen(vga::color::black);
    pit_count = 0;

    IDT::disable_interrupts();

    // HACK: Set up the IDT so we can use the UD2 instruction for panic(), which
    //       causes a processor exception. This allows the exception handler to
    //       dump registers unmodified, shown as they were when the panic
    //       occurred.
    // UD2 (Panic)
    idt.register_handler(6, panic_handler);

    // PIT
    idt.register_handler(32, pit_handler);

    // Keyboard
    idt.register_handler(33, kbd_handler);

    // Load the populated IDT into the core.
    IDT::install(&idt);

    gLog.info("Enabling PIC...\n");
    PIC::remap(0x20, 0x28);
    PIC::disable_all();
    PIC::enable_irq(0x0);
    PIC::enable_irq(0x1);

    IDT::enable_interrupts();

    while(true) {
        // Loop forever
    }

    PANIC("End of kmain reached!");
    return -1;
}