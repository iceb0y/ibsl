#ifndef IBSL_MEMORY_H
#define IBSL_MEMORY_H

#include <atomic>
#include <cstdint>
#include <new>
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
        return Ptr<T>(new (std::nothrow) T(std::forward<ArgsT>(args)...));
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
class BasicRcWrapper : public CompressedMember<CounterT, Tag<0>>,
                       public CompressedMember<ObjectT, Tag<1>> {
public:
    template <typename... ArgsT>
    BasicRcWrapper(ArgsT &&...args)
            : CompressedMember<CounterT, Tag<0>>(1),
              CompressedMember<ObjectT, Tag<1>>(std::forward<ArgsT>(args)...)
    {}

    void AddRef() {
        ++CompressedMember<CounterT, Tag<0>>::object();
    }

    void Release() {
        if (--CompressedMember<CounterT, Tag<0>>::object() == 0) {
            delete this;
        }
    }
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

    BasicRc(const BasicRc<ObjectT, CounterT> &other)
            : wrapper_(other.wrapper_) {
        wrapper_->AddRef();
    }

    BasicRc<ObjectT, CounterT> &operator=(
            const BasicRc<ObjectT, CounterT> &other) {
        if (wrapper_) {
            wrapper_->Release();
        }
        wrapper_ = other.wrapper_;
        wrapper_->AddRef();
        return *this;
    };

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
                new (std::nothrow) BasicRcWrapper<ObjectT, CounterT>(
                        std::forward<ArgsT>(args)...));
    }

    explicit operator bool() const {
        return wrapper_ != nullptr;
    }

    ObjectT &operator *() const {
        return wrapper_->CompressedMember<ObjectT, Tag<1>>::object();
    }

    ObjectT *operator->() const {
        return &wrapper_->CompressedMember<ObjectT, Tag<1>>::object();
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
