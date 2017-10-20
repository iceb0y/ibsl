#ifndef IBSL_LINUX_IO_H
#define IBSL_LINUX_IO_H

#include <errno.h>
#include <unistd.h>
#include "ibsl/view.h"
#include "ibsl/linux/status.h"

namespace ibsl {

template <int fd>
struct LinuxBasicInput {
    LinuxStatus Read(MutableStringView buffer, size_t &actual_size);
};

template <int fd>
LinuxStatus LinuxBasicInput<fd>::Read(MutableStringView buffer,
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
    LinuxStatus Write(StringView buffer, size_t &actual_size);
};

template <int fd>
LinuxStatus LinuxBasicOutput<fd>::Write(StringView buffer,
                                        size_t &actual_size) {
    ssize_t result = write(fd, buffer.data(), buffer.size());
    if (result < 0) {
        return LinuxStatus(errno);
    }
    actual_size = result;
    return LinuxStatus();
}

using LinuxStandardInput = LinuxBasicInput<STDIN_FILENO>;
using LinuxStandardOutput = LinuxBasicOutput<STDOUT_FILENO>;
using LinuxStandardError = LinuxBasicOutput<STDERR_FILENO>;

using StandardOutput = BufferedOutput<LinuxStandardOutput>;
using StandardError = BufferedOutput<LinuxStandardError>;

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
