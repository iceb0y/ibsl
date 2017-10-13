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
    bool IsSuccess();

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
bool BasicStatus<CategoryT>::IsSuccess() {
    return CategoryT::IsSuccess(value_);
}

}

#endif
