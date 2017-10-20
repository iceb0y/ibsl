#include <cassert>
#include <ibsl.h>

using namespace ibsl;

int main() {
    // TODO(iceboy): Replace assert with check when available.
    // TODO(iceboy): Use a unit test framework when available.
    assert(Status(StatusValue::kSuccess).success());
    assert(!Status(StatusValue::kUnknown).success());

    Buffer buffer;
    assert(buffer.Init(42).success());
    assert(buffer.size() == 42);
    assert(buffer.Expand(96).success());
    assert(buffer.size() == 168);

    BufferedOutput<LinuxStandardOutput, 4096> stdout;
    stdout.Init(8192);
    WriteText(stdout, "Hello ", 42);
}
