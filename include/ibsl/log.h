#ifndef IBSL_LOG_H
#define IBSL_LOG_H

#include <cstdlib>
#include "ibsl/io.h"
#include "ibsl/util.h"

#define _IBSL_STRINGIZE(x) _IBSL_STRINGIZE2(x)
#define _IBSL_STRINGIZE2(x) #x

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            WriteText( \
                Singleton<StandardError>::instance(), \
                StringLiteral("Check failed: " #condition " at " \
                              __FILE__ ":" _IBSL_STRINGIZE(__LINE__) "\n")); \
            exit(1); \
        } \
    } while (0)

#endif
