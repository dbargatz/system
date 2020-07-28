#ifndef _STD_QUEUE_HPP
#define _STD_QUEUE_HPP

#include <cassert.hpp>
#include <cstdint.hpp>
#include <cstring.hpp>

namespace std {

/**
 * Simple queue based on a stack-allocated circular buffer.
 * 
 * @tparam T type of item this queue will store
 * @tparam N maximum number of items this queue can hold
 * @note Internally, this queue allocates space for N+1 items on the stack to
 *       simplify size calculations
 */
template <typename T, std::uint16_t N>
class queue {
public:
    /**
     * Construct a new queue object with the backing store on the stack.
     */
    queue() : _front(0), _back(0) {
        std::memset(_buf, 0, sizeof(T) * (N+1));
    }

    /**
     * Remove and return the item at the front of the queue.
     *
     * @return T item at the front of the queue
     * @warning Will panic if there are no items in the queue
     */
    T dequeue() {
        assert(size() > 0);

        // Grab the item, then increment and wrap the index around the buffer if
        // necessary, taking into account the sentinel space (N+1).
        T item = _buf[_front];
        _front = (_front + 1) % (N + 1);
        return item;
    }

    /**
     * Add the given item to the back of the queue.
     *
     * @param in_item item to add at the back of the queue
     * @warning Will panic if the queue is full
     */
    void enqueue(T in_item) {
        assert(size() < N);

        // Store the item at the back of the queue, then increment and wrap the
        // index around the buffer if necessary, taking into account the
        // sentinel space (N+1).
        _buf[_back] = in_item;
        _back = (_back + 1) % (N + 1);
    }

    /**
     * Get the item at the front of the queue without removing it.
     *
     * @return T item at the front of the queue
     * @warning Will panic if there are no items in the queue
     */
    T& peek() {
        assert(size() > 0);
        return _buf[_front];
    }

    /**
     * Return the number of items in the queue.
     * 
     * @return size_t number of items in the queue
     */
    std::size_t size() {
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
    std::size_t _front;

    ///< Index of the item at the back (tail) of the queue. This was the last
    ///< item to be enqueued().
    std::size_t _back;
};

}; // namespace std

#endif // _STD_QUEUE_HPP
