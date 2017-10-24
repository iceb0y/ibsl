#ifndef IBSL_TEXT_H
#define IBSL_TEXT_H

#include <cstdint>

namespace ibsl {

template <typename OutputT, typename ValueT, size_t buffer_size>
Status WriteTextForUnsignedInteger(OutputT &output, ValueT value) {
    size_t actual_size;
    if (value == 0) {
        return output.Write(StringLiteral("0"), actual_size);
    }
    char buffer[buffer_size];
    char *head = &buffer[buffer_size];
    do {
        *--head = '0' + value % 10;
        value /= 10;
    } while (value);
    return output.Write({head, &buffer[buffer_size] - head}, actual_size);
};

template <typename OutputT, typename ValueT, size_t buffer_size>
Status WriteTextForSignedInteger(OutputT &output, ValueT value) {
    char buffer[buffer_size];
    char *head = &buffer[buffer_size];
    size_t actual_size;
    if (value > 0) {
        do {
            *--head = '0' + value % 10;
            value /= 10;
        } while (value);
        return output.Write({head, &buffer[buffer_size] - head}, actual_size);
    } else if (value == 0) {
        return output.Write(StringLiteral("0"), actual_size);
    } else {
        do {
            *--head = '0' - value % 10;
            value /= 10;
        } while (value);
        *--head = '-';
        return output.Write({head, &buffer[buffer_size] - head}, actual_size);
    }
};

template <typename OutputT>
Status WriteText(OutputT &output, int32_t value) {
    return WriteTextForSignedInteger<OutputT, int32_t, 12>(output, value);
}

template <typename OutputT>
Status WriteText(OutputT &output, uint32_t value) {
    return WriteTextForUnsignedInteger<OutputT, uint32_t, 12>(output, value);
}

template <typename OutputT>
Status WriteText(OutputT &output, int64_t value) {
    return WriteTextForSignedInteger<OutputT, int64_t, 20>(output, value);
}

template <typename OutputT>
Status WriteText(OutputT &output, uint64_t value) {
    return WriteTextForUnsignedInteger<OutputT, uint64_t, 20>(output, value);
}

}

#endif
