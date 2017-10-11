#ifndef IBSL_STATUS_H
#define IBSL_STATUS_H

namespace ibsl {

template <typename CategoryT>
class Status {
public:
    using ValueType = typename CategoryT::ValueType;
    static constexpr ValueType kSuccessValue = CategoryT::kSuccessValue;

    Status();
    Status(ValueType value);
    bool IsSuccess();

private:
    ValueType value;
};

template <typename CategoryT>
Status<CategoryT>::Status()
    : value(kSuccessValue) {}

template <typename CategoryT>
Status<CategoryT>::Status(ValueType value)
    : value(value) {}

template <typename CategoryT>
bool Status<CategoryT>::IsSuccess() {
    return CategoryT::IsSuccess(value);
}

}

#endif
