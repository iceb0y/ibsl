#ifndef IBSL_UTIL_H
#define IBSL_UTIL_H

#include <cstdlib>
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

#endif
