#ifndef IBSL_TYPE_H
#define IBSL_TYPE_H

#include "ibsl/view.h"

namespace ibsl {

class StringLiteral : public StringView {
public:
    template <size_t size>
    constexpr StringLiteral(const char (&s)[size])
        : StringView(s, size - 1) {}
};

}

#endif
