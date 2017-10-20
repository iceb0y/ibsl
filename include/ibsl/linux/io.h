#ifndef IBSL_LINUX_IO_H
#define IBSL_LINUX_IO_H

#include <errno.h>
#include <unistd.h>
#include "ibsl/view.h"
#include "ibsl/linux/status.h"

namespace ibsl {

template <int fd>
struct BasicRawInput {
    LinuxStatus Read(MutableStringView buffer, size_t &actual_size);
};

template <int fd>
LinuxStatus BasicRawInput<fd>::Read(MutableStringView buffer,
                                      size_t &actual_size) {
    ssize_t result = read(fd, buffer.data(), buffer.size());
    if (result < 0) {
        return LinuxStatus(errno);
    }
    actual_size = result;
    return LinuxStatus();
}

template <int fd>
struct BasicRawOutput {
    LinuxStatus Write(StringView buffer, size_t &actual_size);
};

template <int fd>
LinuxStatus BasicRawOutput<fd>::Write(StringView buffer,
                                        size_t &actual_size) {
    ssize_t result = write(fd, buffer.data(), buffer.size());
    if (result < 0) {
        return LinuxStatus(errno);
    }
    actual_size = result;
    return LinuxStatus();
}

using RawStandardInput = BasicRawInput<STDIN_FILENO>;
using RawStandardOutput = BasicRawOutput<STDOUT_FILENO>;
using RawStandardError = BasicRawOutput<STDERR_FILENO>;

static constexpr size_t kIoBlockSize = 4096;

using StandardOutput = BufferedOutput<RawStandardOutput, kIoBlockSize>;
using StandardError = BufferedOutput<RawStandardError, kIoBlockSize>;

}

#endif
