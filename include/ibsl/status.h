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

enum StatusValue {
    STATUS_SUCCESS = 0,
    STATUS_UNKNOWN = 1,
    STATUS_CANCELLED = 2,
    STATUS_UNAVAILABLE = 3,
    STATUS_PENDING = 4,
};

struct StatusCategory {
    using ValueType = StatusValue;

    static bool IsSuccess(StatusValue value) {
        return value == kSuccessValue;
    }

    static constexpr StatusValue kSuccessValue = STATUS_SUCCESS;
};

using Status = BasicStatus<StatusCategory>;

}

#endif
