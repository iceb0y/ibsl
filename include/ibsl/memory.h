#ifndef IBSL_MEMORY_H
#define IBSL_MEMORY_H

#include <atomic>
#include <cstdint>
#include <utility>

namespace ibsl {

template <typename T>
class Ptr {
public:
    Ptr() : object_(nullptr) {}
    ~Ptr() { delete object_; }

    Ptr(const Ptr<T> &) = delete;
    Ptr<T> &operator=(const Ptr<T> &) = delete;

    Ptr(Ptr<T> &&other)
            : object_(other.object_) {
        other.object_ = nullptr;
    }

    Ptr<T> &operator=(Ptr<T> &&other) {
        delete object_;
        object_ = other.object_;
        other.object_ = nullptr;
        return *this;
    }

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

template <typename ObjectT, typename CounterT>
class BasicRcWrapper : public ObjectT {
public:
    template <typename... ArgsT>
    BasicRcWrapper(ArgsT &&...args)
            : ObjectT(std::forward<ArgsT>(args)...),
              counter_(1)
    {}

    void AddRef() {
        ++counter_;
    }

    void Release() {
        if (--counter_ == 0) {
            delete this;
        }
    }

private:
    CounterT counter_;
};

template <typename ObjectT, typename CounterT>
class BasicRc {
public:
    BasicRc() : wrapper_(nullptr) {}

    ~BasicRc() {
        if (wrapper_) {
            wrapper_->Release();
        }
    }

    BasicRc(const BasicRc<ObjectT, CounterT> &) = delete;
    BasicRc<ObjectT, CounterT> &operator=(
            const BasicRc<ObjectT, CounterT> &) = delete;

    BasicRc(BasicRc<ObjectT, CounterT> &&other)
            : wrapper_(other.wrapper_) {
        other.wrapper_ = nullptr;
    }

    BasicRc<ObjectT, CounterT> &operator=(BasicRc<ObjectT, CounterT> &&other) {
        if (wrapper_) {
            wrapper_->Release();
        }
        wrapper_ = other.wrapper_;
        other.wrapper_ = nullptr;
        return *this;
    }

    template <typename... ArgsT>
    static BasicRc<ObjectT, CounterT> New(ArgsT &&...args) {
        return BasicRc<ObjectT, CounterT>(
                new BasicRcWrapper<ObjectT, CounterT>(
                        std::forward<ArgsT>(args)...));
    }

    explicit operator bool() const {
        return wrapper_ != nullptr;
    }

    ObjectT &operator *() const {
        return *wrapper_;
    }

    ObjectT *operator->() const {
        return wrapper_;
    }

private:
    explicit BasicRc(BasicRcWrapper<ObjectT, CounterT> *wrapper)
        : wrapper_(wrapper) {}

    BasicRcWrapper<ObjectT, CounterT> *wrapper_;
};

template <typename ObjectT>
using Rc = BasicRc<ObjectT, uint32_t>;

template <typename ObjectT>
using Arc = BasicRc<ObjectT, std::atomic<uint32_t>>;

}

#endif
