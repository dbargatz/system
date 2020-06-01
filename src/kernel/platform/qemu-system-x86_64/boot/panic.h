#ifndef PANIC_H
#define PANIC_H

#define PANIC(msg)                                         \
do {                                                       \
    asm volatile("ud2                          \n"         \
                 ".quad %c0                    \n"         \
                 ".word %c1                    \n"         \
                 ".quad %c2                    \n"         \
                 : : "i" (__FILE__), "i" (__LINE__),       \
                     "i" (msg));                           \
} while(0)

#endif // PANIC_H