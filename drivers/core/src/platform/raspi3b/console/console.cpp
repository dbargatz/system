#include "../../../console/console.hpp"

#include <cstdint>
#include "../platform.hpp"

constexpr std::uint32_t GPFSEL1 = (MMIO_BASE_PHYSADDR + 0x00200004);
// constexpr std::uint32_t GPFSET0   = (MMIO_BASE_PHYSADDR + 0x0020001C);
// constexpr std::uint32_t GPFCLR0   = (MMIO_BASE_PHYSADDR + 0x00200028);
constexpr std::uint32_t GPPUD = (MMIO_BASE_PHYSADDR + 0x00200094);
constexpr std::uint32_t GPPUDCLK0 = (MMIO_BASE_PHYSADDR + 0x00200094);

constexpr std::uint32_t AUX_ENABLES = (MMIO_BASE_PHYSADDR + 0x00215004);
constexpr std::uint32_t AUX_MU_IO_REG = (MMIO_BASE_PHYSADDR + 0x00215040);
constexpr std::uint32_t AUX_MU_IER_REG = (MMIO_BASE_PHYSADDR + 0x00215044);
constexpr std::uint32_t AUX_MU_IIR_REG = (MMIO_BASE_PHYSADDR + 0x00215048);
constexpr std::uint32_t AUX_MU_LCR_REG = (MMIO_BASE_PHYSADDR + 0x0021504C);
constexpr std::uint32_t AUX_MU_MCR_REG = (MMIO_BASE_PHYSADDR + 0x00215050);
constexpr std::uint32_t AUX_MU_LSR_REG = (MMIO_BASE_PHYSADDR + 0x00215054);
// constexpr std::uint32_t AUX_MU_MSR_REG = (MMIO_BASE_PHYSADDR + 0x00215058);
// constexpr std::uint32_t AUX_MU_SCRATCH = (MMIO_BASE_PHYSADDR + 0x0021505C);
constexpr std::uint32_t AUX_MU_CNTL_REG = (MMIO_BASE_PHYSADDR + 0x00215060);
// constexpr std::uint32_t AUX_MU_STAT_REG = (MMIO_BASE_PHYSADDR + 0x00215064);
constexpr std::uint32_t AUX_MU_BAUD_REG = (MMIO_BASE_PHYSADDR + 0x00215068);

extern "C" void delay_cycles(std::uint32_t in_num_cycles);
extern "C" std::uint32_t get32(std::uint32_t in_mmio_reg);
extern "C" void put32(std::uint32_t in_mmio_reg, std::uint32_t in_value);

bool core::console::console::_platform_init(void) {
    put32(AUX_ENABLES, 1);          // Enable mini uart (this also enables access to its registers)
    put32(AUX_MU_CNTL_REG, 0);      // Disable auto flow control and disable receiver and transmitter (for now)
    put32(AUX_MU_LCR_REG, 3);       // Enable 8 bit mode
    put32(AUX_MU_MCR_REG, 0);       // Set RTS line to be always high
    put32(AUX_MU_IER_REG, 0);       // Disable receive and transmit interrupts
    put32(AUX_MU_IIR_REG, 0xC6);
    put32(AUX_MU_BAUD_REG, 270);    // Set baud rate to 115200

    auto selector = get32(GPFSEL1);
    selector &= ~(7 << 12);         // clean gpio14
    selector |= 2 << 12;            // set alt5 for gpio14
    selector &= ~(7 << 15);         // clean gpio15
    selector |= 2 << 15;            // set alt5 for gpio15
    put32(GPFSEL1, selector);

    put32(GPPUD, 0);
    delay_cycles(150);
    put32(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay_cycles(150);
    put32(GPPUDCLK0, 0);

    // Finally, enable transmitter and receiver.
    put32(AUX_MU_CNTL_REG, 3); 
    return true;
}

void core::console::console::_platform_write(const char in_c) {
    // Wait until the status register indicates the line is clear to send.
    while (0 == (get32(AUX_MU_LSR_REG) & 0x20)) { }

    // Send the given character.
    put32(AUX_MU_IO_REG, in_c);
}

void core::console::console::_platform_write(const char *in_str) {
    for (int i = 0; in_str[i] != '\0'; i++) {
        // Logging only uses newlines (\n), but TTY serial connections expect a
        // carriage return (\r) followed by a newline (\n). Without the \r, the
        // cursor never returns to the start of a line, so messages with just a
        // newline end up looking like this:
        // This is line 1
        //               This is line 2
        //                             See why the \r is needed?
        // To avoid this, put a carriage return before putting any newlines.
        // It's okay if the newline already had a carriage return before it;
        // sending multiple carriage returns does nothing other than waste a
        // bit of time.
        if (in_str[i] == '\n') {
            _platform_write('\r');
        }

        // Send this character.
        _platform_write(in_str[i]);
    }
}
