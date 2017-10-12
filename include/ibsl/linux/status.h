#ifndef IBSL_LINUX_STATUS_H
#define IBSL_LINUX_STATUS_H

#include "ibsl/status.h"

namespace ibsl {

struct LinuxStatusCategory {
    using ValueType = int;

    static bool IsSuccess(int value) {
        return value == kSuccessValue;
    }

    static constexpr int kSuccessValue = 0;
};

using LinuxStatus = BasicStatus<LinuxStatusCategory>;

static_assert(sizeof(LinuxStatus) == sizeof(int));

}

#endif
