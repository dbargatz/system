#include "std/halt.h"
#include "std/logger.hpp"
#include "std/panic.h"
#include "std/text.hpp"
#include "debug/serial.hpp"
#include "interrupts/interrupt_manager.hpp"
#include "core.hpp"

//InterruptManager * gInterrupts;
//uint64_t pit_count;

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

/*extern "C" __attribute__((interrupt))
void pit_handler(struct interrupt_frame * in_frame) {
    logger log(gVga);
    if(pit_count > 0 && 0 == (pit_count % 100)) {
        log.info("PIT has fired {} times.\n", pit_count);
    }
    pit_count++;
    gInterrupts->handler_complete(InterruptType::TIMER_EXPIRED);
}*/

extern "C" int kmain(const void * in_boot_info) {
    SerialPort serial;
    logger log(serial);
    IDT idt(log);
    PIC pic(log);
    InterruptManager intmgr(log, idt, pic);
    Core bootstrap_core(log, in_boot_info, intmgr);

    bootstrap_core.run();
    //gInterrupts = &intmgr;

    //pit_count = 0;

    //intmgr.register_handler(InterruptType::TIMER_EXPIRED, pit_handler);

    PANIC("End of kmain reached!");
    return -1;
}