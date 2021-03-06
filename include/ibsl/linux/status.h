#ifndef IBSL_LINUX_STATUS_H
#define IBSL_LINUX_STATUS_H

#include "ibsl/status.h"

namespace ibsl {

struct LinuxStatusCategory {
    using ValueType = int;
    static constexpr int kSuccessValue = 0;

    static bool IsSuccess(int value) {
        return value == kSuccessValue;
    }
};

using LinuxStatus = BasicStatus<LinuxStatusCategory>;

static_assert(sizeof(LinuxStatus) == sizeof(int));

template <>
inline Status ToStatus<LinuxStatus>(LinuxStatus status) {
    switch (status.value()) {
    case 0:
        return Status();
    default:
        return Status(StatusValue::kUnknown);
    }
}

}

#endif
