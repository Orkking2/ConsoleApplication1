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

#define _NSTD_FOR(len) for (int i = 0; i < len; i++)

#define _NSTD_ITSL { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }


#define _NSTD ::nstd::
#define _NSTD_BEGIN namespace nstd {
#define _NSTD_END }



#endif // ifndef _NSTD_DEFINES_