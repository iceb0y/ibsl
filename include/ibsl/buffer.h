#ifndef IBSL_BUFFER_H
#define IBSL_BUFFER_H

#include <cstddef>
#include <cstring>
#include "ibsl/array.h"
#include "ibsl/status.h"
#include "ibsl/view.h"

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

template <typename BaseOutputT, size_t block_size>
class BufferedOutput : private BaseOutputT {
public:
    ~BufferedOutput();
    Status Init(size_t initial_alloc_size);
    Status Write(StringView buffer, size_t &actual_size);
    Status Flush();

private:
    Buffer buffer_;
    size_t data_size_ = 0;
};

template <typename BaseOutputT, size_t block_size>
BufferedOutput<BaseOutputT, block_size>::~BufferedOutput() {
    Flush();  // may fail
}

template <typename BaseOutputT, size_t block_size>
Status BufferedOutput<BaseOutputT, block_size>::Init(
        size_t initial_alloc_size) {
    buffer_.Init(initial_alloc_size);
}

template <typename BaseOutputT, size_t block_size>
Status BufferedOutput<BaseOutputT, block_size>::Write(
        StringView buffer, size_t &actual_size) {
    if (buffer.size() < block_size) {
        Status status = buffer_.Expand(data_size_ + buffer.size());
        if (!status.success()) {
            return status;
        }
        memcpy(buffer_.data() + data_size_, buffer.data(), buffer.size());
        if ((data_size_ += buffer.size()) >= block_size) {
            Flush();  // may fail
        }
        actual_size = buffer.size();
        return Status();
    } else {
        Status status = Flush();
        if (!status.success()) {
            return status;
        }
        return ToStatus(BaseOutputT::Write(buffer, actual_size));
    }
}

template <typename BaseOutputT, size_t block_size>
Status BufferedOutput<BaseOutputT, block_size>::Flush() {
    Status status;
    char *data = buffer_.data();
    size_t remaining = data_size_;
    size_t actual_size;

    while (remaining) {
        status = ToStatus(BaseOutputT::Write({data, remaining}, actual_size));
        if (!status.success()) {
            break;
        }
        data += actual_size;
        remaining -= actual_size;
    }

    memmove(buffer_.data(), buffer_.data() + actual_size, data_size_);
    data_size_ = remaining;
    return status;
}

}

#endif
