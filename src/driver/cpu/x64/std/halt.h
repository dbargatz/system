#ifndef _STD_HALT_H
#define _STD_HALT_H

/**
 * @brief Halts the current core.
 */
static inline void halt(void) {
    // TODO: should this enable interrupts so the core can be woken up?
    asm("hlt");
}

#endif // _STD_HALT_H
