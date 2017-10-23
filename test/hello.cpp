#include <ibsl.h>
#include <ibsl/type.h>

using namespace ibsl;

int main() {
    // TODO(iceboy): Use a unit test framework when available.
    CHECK(Status(StatusValue::kSuccess).success());
    CHECK(!Status(StatusValue::kUnknown).success());

    PrintText(StringLiteral("Hello "), 42, StringLiteral("\n"));
}
