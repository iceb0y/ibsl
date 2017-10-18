#ifndef IBSL_BUFFER_H
#define IBSL_BUFFER_H

#include <cstddef>
#include "ibsl/array.h"
#include "ibsl/status.h"

namespace ibsl {

template <typename ElementT, typename SizeT>
class BasicBuffer {
public:
    Status Init(SizeT initial_size);
    Status Expand(SizeT min_size);

    ElementT *data() const { return array_.data(); }
    SizeT size() const { return size_; }

private:
    BasicPODArray<ElementT, SizeT> array_;
    SizeT size_;
};

template <typename ElementT, typename SizeT>
Status BasicBuffer<ElementT, SizeT>::Init(SizeT initial_size) {
    Status status = array_.Resize(initial_size);
    if (!status.success()) {
        return status;
    }
    size_ = initial_size;
    return Status();
};

template <typename ElementT, typename SizeT>
Status BasicBuffer<ElementT, SizeT>::Expand(SizeT min_size) {
    if (min_size > size_) {
        size_t new_size = size_;
        do {
            new_size *= 2;
        } while (min_size > new_size);
        Status status = array_.Resize(new_size);
        if (!status.success()) {
            return status;
        }
        size_ = new_size;
    }
    return Status();
}

using Buffer = BasicBuffer<char, size_t>;

}

#endif
