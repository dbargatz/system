#ifndef _DEVICETREE_DETAILS_ITERATOR_HPP
#define _DEVICETREE_DETAILS_ITERATOR_HPP

namespace devicetree::details {

template <typename T>
class iterator {
private:
    typename T::internal_struct * _current;

public:
    iterator(typename T::internal_struct * in_ptr) { _current = in_ptr; }
    bool operator==(const iterator<T>& in_other) const { return (_current == in_other._current); }
    bool operator!=(const iterator<T>& in_other) const { return !(*this == in_other); }

    T operator*() { return T(_current); }
    iterator<T>& operator++();
};

}; // namespace devicetree::details

#endif // _DEVICETREE_DETAILS_ITERATOR_HPP