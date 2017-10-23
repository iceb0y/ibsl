#ifndef IBSL_UTIL_H
#define IBSL_UTIL_H

#include <cstdlib>

namespace ibsl {

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};

template <typename T>
class Singleton {
public:
    static T &instance();
};

template <typename T>
T &Singleton<T>::instance() {
    static T kInstance;
    return kInstance;
}

}

#endif
