#ifndef IBSL_STRING_VIEW_H
#define IBSL_STRING_VIEW_H

#include <cstddef>

namespace ibsl {

template <typename ElementT, typename SizeT>
class BasicArrayView {
public:
    BasicArrayView(ElementT *data, SizeT size)
            : data_(data), size_(size) {}

    ElementT *data() {
        return data_;
    }

    const ElementT *data() const {
        return data_;
    }

    size_t size() const {
        return size_;
    }

private:
    ElementT *data_;
    SizeT size_;
};

template <typename ElementT>
using ArrayView = BasicArrayView<ElementT, size_t>;
using StringView = ArrayView<char>;

}

#endif //PROJECT_STRING_VIEW_H
