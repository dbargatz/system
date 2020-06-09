#include "memset.h"

void inline memset(void * inPtr, uint8_t inValue, size_t inNum) {
    // Simple, inefficient, but can be upgraded later.
    for(uint8_t i = 0; i < inNum; i++) {
        ((uint8_t *)inPtr)[i] = inValue;
    }
}
