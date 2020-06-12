#include "std/text.hpp"
#include "std/logger.hpp"
#include "std/panic.h"
#include "debug/serial.hpp"
#include "display/vga.hpp"
#include "interrupts/interrupt_manager.hpp"
#include "core.hpp"

vga gVga;
InterruptManager * gInterrupts;
uint64_t pit_count;

void hexdump(const void * in_ptr, uint8_t in_count) {
    uint8_t * ptr = (uint8_t *)in_ptr;
    vga::color fg = vga::color::white;
    vga::color bg = vga::color::bright_magenta;

    for(int i = 0; i < in_count; i++) {
        if(i % 16 == 0) {
            if(i > 0) {
                gVga.write("\n", fg, bg);
            }
            gVga.write(text("{#016X}:\t",(uint64_t)(ptr+i)).get(), fg, bg);
        }
        gVga.write(text("{02X} ", *(ptr+i)).get(), fg, bg);
    }
    gVga.write("\n", fg, bg);
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
    vga::color fg = vga::color::white;
    vga::color bg = vga::color::bright_magenta;
    struct panic_data * data = (struct panic_data *)in_frame->rip;
    gVga.clear_screen(bg);
    gVga.set_position(0, 0);

    hexdump((void *)data, 64);
    gVga.write(text("PANIC({}:{}): {}\n", data->filename, data->lineNum, data->msg).get(), fg, bg);
    while(true) {
        // Loop forever
    }
}

extern "C" __attribute__((interrupt))
void pit_handler(struct interrupt_frame * in_frame) {
    logger log(gVga);
    if(pit_count > 0 && 0 == (pit_count % 100)) {
        log.info("PIT has fired {} times.\n", pit_count);
    }
    pit_count++;
    gInterrupts->handler_complete(InterruptType::TIMER_EXPIRED);
}

extern "C" int kmain(const void * in_boot_info) {
    logger serial_log(gVga);
    SerialPort serial(serial_log);
    logger idt_log(gVga);
    IDT idt(idt_log);
    logger pic_log(gVga);
    PIC pic(pic_log);
    logger intmgr_log(gVga);
    InterruptManager intmgr(intmgr_log, idt, pic);
    logger core_log(gVga);

    gInterrupts = &intmgr;
    Core bootstrap_core(core_log, in_boot_info, intmgr, serial);

    pit_count = 0;

    intmgr.register_handler(InterruptType::PANIC, panic_handler);
    //intmgr.register_handler(InterruptType::TIMER_EXPIRED, pit_handler);

    while(true) {
        // Loop forever
    }

    PANIC("End of kmain reached!");
    return -1;
}