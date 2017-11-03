#include <ibsl.h>

using namespace ibsl;

int main() {
    auto p = Ptr<int>::New(70514);
    return *p;
}
