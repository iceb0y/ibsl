#ifndef IBSL_UTIL_H
#define IBSL_UTIL_H

#include <cstdlib>
#include <type_traits>
#include <utility>

namespace ibsl {

class DefaultTag {};

template <int index>
class Tag {};

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};

template <typename InstanceT, typename TagT = DefaultTag>
class Singleton {
public:
    static InstanceT &instance();
};

template <typename InstanceT, typename TagT>
InstanceT &Singleton<InstanceT, TagT>::instance() {
    static InstanceT kInstance;
    return kInstance;
}

template <typename ObjectT, typename TagT, bool use_inherit>
class CompressedMemberImpl;

template <typename ObjectT, typename TagT>
class CompressedMemberImpl<ObjectT, TagT, false> {
public:
    template <typename... ArgsT>
    CompressedMemberImpl(ArgsT &&...args)
            : object_(std::forward<ArgsT>(args)...)
    {}

    ObjectT &object() { return object_; }

private:
    ObjectT object_;
};

template <typename ObjectT, typename TagT>
class CompressedMemberImpl<ObjectT, TagT, true> : private ObjectT {
public:
    template <typename... ArgsT>
    CompressedMemberImpl(ArgsT &&...args)
            : ObjectT(std::forward<ArgsT>(args)...)
    {}

    ObjectT &object() { return *this; }
};

template <typename ObjectT, typename TagT = DefaultTag>
using CompressedMember = CompressedMemberImpl<
        ObjectT, TagT, std::is_class<ObjectT>::value>;

}

// Wraps a class into POD, which needs explicit creation and deletion.
template <typename T>
class PodWrapper {
public:
    template <typename ...ArgsT>
    void emplace(ArgsT &&...args) { new(&storage_) T(std::forward<ArgsT>(args)...); }
    void destroy() { reinterpret_cast<T *>(&storage_)->~T(); }
    T &get() { return *reinterpret_cast<T *>(&storage_); }
    const T &get() const { return *reinterpret_cast<const T *>(&storage_); }
    T *operator->() { return &get(); }
    const T *operator->() const { return &get(); }

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;
};

#endif
