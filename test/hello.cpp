#include <cassert>
#include <ibsl.h>
#include <cstdio>

using namespace ibsl;

int main() {
    // TODO(iceboy): Replace assert with check when available.
    // TODO(iceboy): Use a unit test framework when available.
    assert(Status(StatusValue::kSuccess).success());
    assert(!Status(StatusValue::kUnknown).success());

    auto &stdout = Singleton<LinuxStandardOutput>::instance();
    size_t actual_size;
    stdout.Write({"Hello world!\n", 13}, actual_size);

    Buffer buffer;
    assert(buffer.Init(42).success());
    assert(buffer.size() == 42);
    assert(buffer.Expand(96).success());
    assert(buffer.size() == 168);
}
