#ifndef _STD_ASSERT_H
#define _STD_ASSERT_H

#include "panic.h"

/**
 * If @cond is @false, triggers a PANIC().
 *
 * @param cond: the condition to test.
 * @param msg: the message to provide to PANIC_TYPED().
 */
#define ASSERT(cond, msg)                                         \
if(!(cond)) {                                                     \
    PANIC_TYPED(msg ": !(" #cond ")", panic_type::ASSERT_FAILED); \
}

#endif // _STD_ASSERT_H
