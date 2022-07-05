#pragma once

// Get STL defaults
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
typedef unsigned long long uint; 
typedef unsigned      char uchar;

_NSTD_END

#define _NSTD_UNSIGNED		  \
	typedef _NSTD uint uint;  \
	typedef _NSTD uchar uchar;



// Vars
#define _I i_love_u_mom
#define _J jeepers_steeve
#define _K keepers_steeve

#define _NSTD_FOR(var_name, length) for (_NSTD uint var_name = 0; var_name < length; var_name++)
#define _NSTD_FOR_I(len) _NSTD_FOR(_I, len)
#define _NSTD_FOR_J(len) _NSTD_FOR(_J, len)
#define _NSTD_FOR_K(len) _NSTD_FOR(_K, len)

// type must have a default constructor
#define _GET_BIT(type, index) ((type() & char(0) | char(1)) << (index))
#define _BITX(index)    _GET_BIT(_NSTD uchar, index)
#define _HIGH_BIT(type) _GET_BIT(type, sizeof(type) * CHAR_BIT - 1)


#endif // ifndef _NSTD_DEFINES_