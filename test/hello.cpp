#include <cassert>
#include <ibsl.h>

using namespace ibsl;

int main() {
    assert(LinuxStatus(0).IsSuccess());
    assert(!LinuxStatus(1).IsSuccess());
}
