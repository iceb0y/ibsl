#ifndef IBSL_STATUS_H
#define IBSL_STATUS_H

namespace ibsl {

template <typename CategoryT>
class BasicStatus {
public:
    using ValueType = typename CategoryT::ValueType;
    static constexpr ValueType kSuccessValue = CategoryT::kSuccessValue;

    BasicStatus();
    explicit BasicStatus(ValueType value);
    bool success();

private:
    ValueType value_;
};

template <typename CategoryT>
BasicStatus<CategoryT>::BasicStatus()
    : value_(kSuccessValue) {}

template <typename CategoryT>
BasicStatus<CategoryT>::BasicStatus(ValueType value)
    : value_(value) {}

template <typename CategoryT>
bool BasicStatus<CategoryT>::success() {
    return CategoryT::IsSuccess(value_);
}

enum class StatusValue {
    kSuccess = 0,
    kUnknown = 1,
    kCancelled = 2,
    kUnavailable = 3,
    kPending = 4,
};

struct StatusCategory {
    using ValueType = StatusValue;
    static constexpr StatusValue kSuccessValue = StatusValue::kSuccess;

    static bool IsSuccess(StatusValue value) {
        return value == kSuccessValue;
    }
};

using Status = BasicStatus<StatusCategory>;

static_assert(sizeof(Status) == sizeof(int));

}

#endif
