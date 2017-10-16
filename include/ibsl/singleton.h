#ifndef IBSL_SINGLETON_H
#define IBSL_SINGLETON_H

namespace ibsl {

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
