#include <cassert>
#include <ibsl.h>

using namespace ibsl;

int main() {
    assert(Status(STATUS_SUCCESS).success());
    assert(!Status(STATUS_UNKNOWN).success());

    auto &stdout = Singleton<LinuxStandardOutput>::instance();
    size_t actual_size;
    stdout.Write({"Hello world!\n", 13}, actual_size);
}
