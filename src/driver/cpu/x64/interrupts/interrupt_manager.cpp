#include "interrupt_manager.hpp"
#include "frame.hpp"
#include "../std/cpuid.h"
#include "../std/halt.h"
#include "../std/panic.h"

#define HANDLERS \
    X(0) X(10) X(20) X(30) X(40) X(50) X(60) X(70) X(80) X(90) X(100) X(110) X(120) X(130) X(140) X(150) X(160) X(170) X(180) X(190) X(200) X(210) X(220) X(230) X(240) X(250) \
    X(1) X(11) X(21) X(31) X(41) X(51) X(61) X(71) X(81) X(91) X(101) X(111) X(121) X(131) X(141) X(151) X(161) X(171) X(181) X(191) X(201) X(211) X(221) X(231) X(241) X(251) \
    X(2) X(12) X(22) X(32) X(42) X(52) X(62) X(72) X(82) X(92) X(102) X(112) X(122) X(132) X(142) X(152) X(162) X(172) X(182) X(192) X(202) X(212) X(222) X(232) X(242) X(252) \
    X(3) X(13) X(23) X(33) X(43) X(53) X(63) X(73) X(83) X(93) X(103) X(113) X(123) X(133) X(143) X(153) X(163) X(173) X(183) X(193) X(203) X(213) X(223) X(233) X(243) X(253) \
    X(4) X(14) X(24) X(34) X(44) X(54) X(64) X(74) X(84) X(94) X(104) X(114) X(124) X(134) X(144) X(154) X(164) X(174) X(184) X(194) X(204) X(214) X(224) X(234) X(244) X(254) \
    X(5) X(15) X(25) X(35) X(45) X(55) X(65) X(75) X(85) X(95) X(105) X(115) X(125) X(135) X(145) X(155) X(165) X(175) X(185) X(195) X(205) X(215) X(225) X(235) X(245) X(255) \
    X(6) X(16) X(26) X(36) X(46) X(56) X(66) X(76) X(86) X(96) X(106) X(116) X(126) X(136) X(146) X(156) X(166) X(176) X(186) X(196) X(206) X(216) X(226) X(236) X(246)        \
    X(7) X(17) X(27) X(37) X(47) X(57) X(67) X(77) X(87) X(97) X(107) X(117) X(127) X(137) X(147) X(157) X(167) X(177) X(187) X(197) X(207) X(217) X(227) X(237) X(247)        \
    X(8) X(18) X(28) X(38) X(48) X(58) X(68) X(78) X(88) X(98) X(108) X(118) X(128) X(138) X(148) X(158) X(168) X(178) X(188) X(198) X(208) X(218) X(228) X(238) X(248)        \
    X(9) X(19) X(29) X(39) X(49) X(59) X(69) X(79) X(89) X(99) X(109) X(119) X(129) X(139) X(149) X(159) X(169) X(179) X(189) X(199) X(209) X(219) X(229) X(239) X(249)        \

#define X(n) extern void * interrupt_handler_##n;
HANDLERS
#undef X

extern "C" void panic_handler(logger& in_log, interrupt_frame& in_frame) {
    struct panic_data * data = (struct panic_data *)in_frame.frame->rip;

    in_log.panic("\n");
    in_log.panic("\n");
    in_log.panic("PANIC({}:{}): {}\n", data->filename, data->lineNum, data->msg);
    in_frame.dump(logger::level::Panic);
    in_log.panic("Raw panic data:\n");
    in_log.hexdump(logger::level::Panic, (void *)data, sizeof(*data), 1);
    in_log.panic("\n");
    in_log.panic("\n");
    halt();
}

extern "C" void unhandled_interrupt_handler(logger& in_log, interrupt_frame& in_frame) {
    in_log.panic("\n");
    in_log.panic("\n");
    in_log.panic("UNHANDLED INTERRUPT: {}\n", in_frame.frame->interrupt_number);
    in_frame.dump(logger::level::Panic);
    in_log.panic("\n");
    in_log.panic("\n");
}

extern "C" void dispatch_interrupt(const void * in_frame_ptr) {
    SerialPort serial;
    logger log(serial);
    interrupt_frame frame(log, in_frame_ptr);

    switch(frame.frame->interrupt_number) {
        case 6:
            panic_handler(log, frame);
            break;
        default:
            unhandled_interrupt_handler(log, frame);
            break;
    }
    log.debug("Back from handler.\n");
}

InterruptManager::InterruptManager(logger& in_log, IDT& in_idt, PIC& in_pic) : 
    _log(in_log), _idt(in_idt), _pic(in_pic) {
    _log.debug("Constructing InterruptManager...\n");
    disable_interrupts();

    // Load the populated IDT into the core.
    _idt.install();

    // Indicate whether a local APIC is present, just for diagnostics.
    uint32_t eax, edx;
    cpuid(1, &eax, &edx);
    _log.debug("This core {} a local APIC.\n",
        (edx & CPUID_01_EDX_LOCAL_APIC_PRESENT) ? "has" : "doesn't have");

    // Remap IRQ 0-7 to IDT vectors 0x20-0x27 (32-39), and IRQ 8-15 to IDT
    // vectors 0x28-0x2F (40-47) to avoid conflicting with the Intel-reserved
    // range of interrupts/exceptions from 0x00-0x1F (0-31).
    _pic.remap(0x20, 0x28);

    // Mask all IRQs (0-15).
    _pic.disable_all();

#define X(n) _idt.register_handler(n, &interrupt_handler_##n);
    HANDLERS
#undef X

    _log.debug("Constructed InterruptManager.\n");
}

void InterruptManager::disable_interrupts() {
    asm volatile("cli");
    _log.debug("Interrupts disabled.\n");
}

void InterruptManager::enable_interrupts() {
    asm volatile("sti");
    _log.debug("Interrupts enabled.\n");
}

bool InterruptManager::enabled() {
    uint64_t rflags;
    asm volatile("pushf; pop %0" : "=g"(rflags));
    return (rflags & RFLAGS_INTERRUPTS_ENABLED_BIT);
}

void InterruptManager::handler_complete(InterruptType in_interrupt) {
    switch(in_interrupt) {
        case InterruptType::TIMER_EXPIRED:
            _pic.send_eoi(0);
            break;
        default:
            _log.warn("Cannot mark handler complete for interrupt {}\n", (uint8_t)in_interrupt);
            break;
    }
}

void InterruptManager::register_handler(InterruptType in_interrupt, const interrupt_handler in_handler) {
    // If interrupts are enabled, disable them.
    bool were_enabled = enabled();
    if(were_enabled) {
        disable_interrupts();
    }

    // Install the handler in the IDT.
    // TODO: these handlers should go into a separate table referenced by the interrupt dispatcher.
    //_idt.register_handler((uint8_t)in_interrupt, in_handler);

    // If this was a hardware interrupt handled by the PIC (IRQs 0-15), enable
    // the IRQ in the correct PIC's mask.
    switch(in_interrupt) {
        case InterruptType::TIMER_EXPIRED:
            _pic.enable_irq(0);
            break;
        default:
            break;
    }

    // Re-enable interrupts if they were enabled on entry.
    if(were_enabled) {
        enable_interrupts();
    }
}
