#ifndef IBSL_LINUX_IO_H
#define IBSL_LINUX_IO_H

#include <errno.h>
#include <unistd.h>
#include "ibsl/string-view.h"
#include "ibsl/linux/status.h"

namespace ibsl {

template <int fd>
struct LinuxBasicInput {
    LinuxStatus Read(StringView buffer, size_t &actual_size);
};

template <int fd>
LinuxStatus LinuxBasicInput<fd>::Read(StringView buffer,
                                      size_t &actual_size) {
    ssize_t result = read(fd, buffer.data(), buffer.size());
    if (result < 0) {
        return LinuxStatus(errno);
    }
    actual_size = result;
    return LinuxStatus();
}

template <int fd>
struct LinuxBasicOutput {
    LinuxStatus Write(const StringView buffer, size_t &actual_size);
};

template <int fd>
LinuxStatus LinuxBasicOutput<fd>::Write(const StringView buffer,
                                        size_t &actual_size) {
    ssize_t result = write(fd, buffer.data(), buffer.size());
    if (result < 0) {
        return LinuxStatus(errno);
    }
    actual_size = result;
    return LinuxStatus();
}

using LinuxRawStandardInput = LinuxBasicInput<STDIN_FILENO>;
using LinuxRawStandardOutput = LinuxBasicOutput<STDOUT_FILENO>;
using LinuxRawStandardError = LinuxBasicOutput<STDERR_FILENO>;

}

#endif
