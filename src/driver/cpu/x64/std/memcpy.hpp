#ifndef _STD_MEMCPY_HPP
#define _STD_MEMCPY_HPP

// TODO: redirect to local stdint.h when migration complete.
#include "../../../../kernel/platform/qemu-system-x86_64/types/stdint.h"

/**
 * @brief Copies the given number of bytes from the source to destination.
 * 
 * @param in_destination pointer to copy bytes to; must not overlap with in_source
 * @param in_source pointer to copy bytes from; must not overlap with in_destination
 * @param in_num number of bytes to copy
 * @return void* in_destination
 */
void * memcpy(void * in_destination, const void * in_source, size_t in_num);

#endif // _STD_MEMCPY_HPP
