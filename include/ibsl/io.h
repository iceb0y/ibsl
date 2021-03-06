#ifndef IBSL_IO_H
#define IBSL_IO_H

#include <cstdio>
#include <cstring>

#include "ibsl/view.h"

namespace ibsl {

static constexpr size_t kDefaultBlockSize = 4096;

template <typename BaseOutputT, size_t block_size = kDefaultBlockSize>
class BufferedOutput : private BaseOutputT {
public:
    ~BufferedOutput();

    Status Write(StringView buffer, size_t &actual_size);
    Status Flush();

private:
    char buffer_[block_size * 2];
    size_t data_size_ = 0;
};

template <typename BaseOutputT, size_t block_size>
BufferedOutput<BaseOutputT, block_size>::~BufferedOutput() {
    Flush();  // may fail
}

template <typename BaseOutputT, size_t block_size>
Status BufferedOutput<BaseOutputT, block_size>::Write(
        StringView buffer, size_t &actual_size) {
    if (buffer.size() < block_size) {
        memcpy(buffer_ + data_size_, buffer.data(), buffer.size());
        if ((data_size_ += buffer.size()) >= block_size) {
            Status status = Flush();
            if (!status.success()) {
                return status;
            }
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
    char *data = buffer_;
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

    memmove(buffer_, buffer_ + actual_size, data_size_);
    data_size_ = remaining;
    return status;
}

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

#ifdef __linux__
#include "ibsl/linux/io.h"
#elif _WIN32
#include "ibsl/win32/io.h"
#endif

namespace ibsl {

template <typename A>
Status PrintText(const A &a) {
    return WriteText(Singleton<StandardOutput>::instance(), a);
};

template <typename A, typename B>
Status PrintText(const A &a, const B &b) {
    return WriteText(Singleton<StandardOutput>::instance(), a, b);
};

template <typename A, typename B, typename C>
Status PrintText(const A &a, const B &b, const C &c) {
    return WriteText(Singleton<StandardOutput>::instance(), a, b, c);
};

}

#endif
