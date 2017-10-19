#ifndef IBSL_UTIL_H
#define IBSL_UTIL_H

namespace ibsl {

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};

}

#endif
