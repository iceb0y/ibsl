#include <ibsl.h>

using namespace ibsl;

int main() {
    // TODO(iceboy): Use a unit test framework when available.
    CHECK(Status(StatusValue::kSuccess).success());
    CHECK(!Status(StatusValue::kUnknown).success());

    PrintText("Hello ", 42, "\n");
}
