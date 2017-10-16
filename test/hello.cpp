#include <cassert>
#include <ibsl.h>

using namespace ibsl;

int main() {
    assert(Status(STATUS_SUCCESS).IsSuccess());
    assert(!Status(STATUS_UNKNOWN).IsSuccess());

    size_t actual_size;
    LinuxStandardOutput().Write({"Hello world!\n", 13}, actual_size);
}
