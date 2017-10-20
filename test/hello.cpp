#include <cassert>
#include <ibsl.h>

using namespace ibsl;

int main() {
    // TODO(iceboy): Replace assert with check when available.
    // TODO(iceboy): Use a unit test framework when available.
    assert(Status(StatusValue::kSuccess).success());
    assert(!Status(StatusValue::kUnknown).success());

    PrintText("Hello ", 42, "\n");
}
