#include "memset.hpp"

void memset(void * inPtr, uint8_t inValue, size_t inNum) {
    // Simple, inefficient, but can be upgraded later.
    for(size_t i = 0; i < inNum; i++) {
        ((uint8_t *)inPtr)[i] = inValue;
    }
}
