#ifndef PANIC_H
#define PANIC_H

#define panic() while(true) { asm("ud2"); }

#endif //PANIC_H