#ifndef _STD_QUEUE_HPP
#define _STD_QUEUE_HPP

#include "assert.h"
#include "logger.hpp"
#include "memset.hpp"
#include "stdint.h"

template <typename T, uint16_t N>
class queue {
public:
    queue() : _front(0), _back(0) {
        memset(_buf, 0, sizeof(T) * (N+1));
    }

    T dequeue() {
        ASSERT(size() > 0, "no items in queue");

        T item = _buf[_front];

        // Increment and wrap the index around the buffer if necessary.
        _front = (_front + 1) % (N + 1);

        return item;
    }

    void dump(logger& in_log) {
        in_log.debug("Queue: {#016X} ({}/{} items)", (uint64_t)this, size(), N);
    }

    void enqueue(T in_item) {
        ASSERT(size() < N, "queue is full");

        _buf[_back] = in_item;

        // Increment and wrap the index around the buffer if necessary.
        _back = (_back + 1) % (N + 1);
    }

    size_t size() {
        if(_back >= _front) {
            return (_back - _front);
        } else {
            return ((_back + N + 1) - _front);
        }
    }

protected:
    ///< Circular backing buffer; sized as N+1 (rather than N) to make size
    ///< calculations easier.
    T _buf[N+1];

    ///< Index of the item at the front (head) of the queue. This was the first
    ///< item to be enqueued() that has not yet been dequeued().
    size_t _front;

    ///< Index of the item at the back (tail) of the queue. This was the last
    ///< item to be enqueued().
    size_t _back;
};

#endif // _STD_QUEUE_HPP
