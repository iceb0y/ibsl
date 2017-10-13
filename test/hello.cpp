#include <cassert>
#include <ibsl.h>

using namespace ibsl;

int main() {
    assert(Status(STATUS_SUCCESS).IsSuccess());
    assert(!Status(STATUS_UNKNOWN).IsSuccess());
}
