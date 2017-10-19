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

    auto &stdout = Singleton<BufferedOutput<LinuxStandardOutput, 4096>>::instance();
    assert(stdout.Init(8192).success());
    size_t actual_size;
    assert(stdout.Write({"Hello world!\n", 13}, actual_size).success());
}
