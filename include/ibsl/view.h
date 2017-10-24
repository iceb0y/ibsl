#ifndef IBSL_VIEW_H
#define IBSL_VIEW_H

#include <cstddef>

namespace ibsl {

template <typename ElementT, typename SizeT>
class BasicArrayView {
public:
    BasicArrayView(ElementT *data, SizeT size)
            : data_(data), size_(size) {}

    ElementT *data() const {
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
using MutableArrayView = BasicArrayView<ElementT, size_t>;

template <typename ElementT>
using ArrayView = MutableArrayView<const ElementT>;

using MutableStringView = MutableArrayView<char>;
using StringView = ArrayView<char>;

template <size_t size>
constexpr StringView StringLiteral(const char (&s)[size]) {
    return {s, size - 1};
}

}

#endif
