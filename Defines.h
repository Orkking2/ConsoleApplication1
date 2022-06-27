#pragma once

// Aquire STL defaults
#include <vcruntime.h>

#ifndef _NSTD_DEFINES_
#define _NSTD_DEFINES_

#define elif(expr) else if (expr)

#ifndef NULL
#define NULL 0
#endif // ifndef NULL

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif // _NODISCARD

#define _NSTD_ASSERT(cond, msg) _STL_VERIFY(cond, msg)


#define _NSTD_ITSL { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }
#define _PREFIX 0

#define _NSTD ::nstd::
#define _NSTD_BEGIN namespace nstd {
#define _NSTD_END }
#define _NSTD_MULTI _NSTD multithreaded::
#define _MULTITHREAD_BEGIN namespace multithreaded {
#define _MULTITHREAD_END }

// NSTD UNIFORM STANDARDS
_NSTD_BEGIN 

// typedefs
typedef size_t  uint; 
typedef unsigned char uchar;

_NSTD_END

#define _NSTD_FOR(variable_name, length) for (_NSTD uint variable_name = 0; variable_name < length; variable_name++)
#define _NSTD_FOR_I(len) _NSTD_FOR(i, len)
#define _NSTD_FOR_J(len) _NSTD_FOR(j, len)

#define _BITX(index) (uchar(1) << (index))


#endif // ifndef _NSTD_DEFINES_