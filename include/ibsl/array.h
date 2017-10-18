#ifndef IBSL_ARRAY_H
#define IBSL_ARRAY_H

#include <cstddef>
#include <cstdlib>
#include <type_traits>
#include "ibsl/status.h"

namespace ibsl {

template <typename ElementT, typename SizeT>
class BasicPODArray {
    static_assert(std::is_pod<ElementT>::value);

public:
    BasicPODArray() : data_(nullptr) {}
    ~BasicPODArray();
    Status Resize(SizeT size);

    ElementT *data() const { return data_; }

private:
    ElementT *data_;
};

template <typename ElementT, typename SizeT>
BasicPODArray<ElementT, SizeT>::~BasicPODArray() {
    free(data_);
}

template <typename ElementT, typename SizeT>
Status BasicPODArray<ElementT, SizeT>::Resize(SizeT size) {
    ElementT *new_data = static_cast<ElementT *>(
            realloc(data_, size * sizeof(ElementT)));
    if (!new_data) {
        return Status(StatusValue::kNoMemory);
    }
    data_ = new_data;
    return Status();
}

using PODArray = BasicPODArray<char, size_t>;

}

#endif
