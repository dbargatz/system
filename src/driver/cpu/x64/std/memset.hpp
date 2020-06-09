#ifndef _STD_MEMSET_H
#define _STD_MEMSET_H

// TODO: redirect to local stdint.h when migration complete.
#include "../../../../kernel/platform/qemu-system-x86_64/types/stdint.h"

/**
 * @brief Sets the given number of bytes at the given pointer to the given 
 * value.
 * 
 * @param inPtr start of bytes to write to
 * @param inValue value of each byte written
 * @param inNum number of bytes to overwrite
 */
void memset(void * inPtr, uint8_t inValue, size_t inNum);

#endif // _STD_MEMSET_H
