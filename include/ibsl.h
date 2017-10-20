#ifndef IBSL_H
#define IBSL_H

#include "ibsl/array.h"
#include "ibsl/io.h"
#include "ibsl/status.h"
#include "ibsl/util.h"
#include "ibsl/view.h"

#ifdef __linux__
#include "ibsl/linux.h"
#elif _WIN32
#include "ibsl/win32.h"
#endif

#endif
