#ifndef IBSL_MEMORY_H
#define IBSL_MEMORY_H

#include <utility>

namespace ibsl {

template <typename T>
class Ptr {
public:
    Ptr() : object_(nullptr) {}
    ~Ptr() { delete object_; }

    template <typename... ArgsT>
    static Ptr<T> New(ArgsT &&...args) {
        return Ptr<T>(new T(std::forward<ArgsT>(args)...));
    }

    explicit operator bool() const {
        return object_ != nullptr;
    }

    T &operator *() const {
        return *object_;
    }

    T *operator->() const {
        return object_;
    }

private:
    explicit Ptr(T *object) : object_(object) {}

    T *object_;
};

}

#endif
