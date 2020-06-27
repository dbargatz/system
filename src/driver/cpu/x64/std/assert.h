#ifndef _STD_ASSERT_H
#define _STD_ASSERT_H

#include "panic.h"
#include "stdint.h"

/**
 * If @cond is @false, triggers a PANIC().
 *
 * @param cond: the condition to test.
 * @param msg: the message to provide to PANIC().
 */
#define ASSERT(cond, msg) if(!(cond)) { PANIC("ASSERTION FAILED (" #msg "): !(" #cond ")"); }

#endif // _STD_ASSERT_H