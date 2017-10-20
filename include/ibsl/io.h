#ifndef IBSL_IO_H
#define IBSL_IO_H

#include <cstdio>

#include "ibsl/view.h"

namespace ibsl {

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

template <typename OutputT>
Status WriteText(OutputT &output, int i) {
    // TODO(iceboy): faster
    char buf[12];
    size_t actual_size;
    return output.Write({buf, sprintf(buf, "%d", i)}, actual_size);
}

template <typename OutputT>
Status WriteText(OutputT &output, const char *str) {
    size_t actual_size;
    return output.Write({str, strlen(str)}, actual_size);
};

template <typename OutputT>
Status WriteText(OutputT &output, StringView view) {
    size_t actual_size;
    return output.Write(view, actual_size);
}

template <typename OutputT, typename A, typename B>
Status WriteText(OutputT &output, const A &a, const B &b) {
    Status status = WriteText(output, a);
    if (!status.success()) {
        return status;
    }
    return WriteText(output, b);
}

template <typename OutputT, typename A, typename B, typename C>
Status WriteText(OutputT &output, const A &a, const B &b, const C &c) {
    Status status = WriteText(output, a);
    if (!status.success()) {
        return status;
    }
    status = WriteText(output, b);
    if (!status.success()) {
        return status;
    }
    return WriteText(output, c);
}

}

#endif
