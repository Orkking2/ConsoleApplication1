#pragma once
#ifndef _NSTD_DEFINES_
#define _NSTD_DEFINES_

#define elif(expr) else if (expr)

#ifndef NULL
#define NULL 0
#endif // ifndef NULL

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif // _NODISCARD

#define _NSTD_FOR(variable_name, len) for (unsigned int variable_name = 0; variable_name < len; variable_name++)
#define _NSTD_FOR_I(len) _NSTD_FOR(i, len)
#define _NSTD_FOR_J(len) _NSTD_FOR(j, len)

#define _NSTD_ITSL { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }

#define _NSTD ::nstd::
#define _NSTD_BEGIN namespace nstd {
#define _NSTD_END }
#define _MULTITHREAD_BEGIN namespace multithreaded {
#define _MULTITHREAD_END }


#endif // ifndef _NSTD_DEFINES_