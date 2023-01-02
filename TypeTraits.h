#pragma once
#ifndef _NSTD_TYPETRAITS_
#define _NSTD_TYPETRAITS_

#include <type_traits>
#include "Defines.h"

// Things I thought could be useful but aren't in <type_traits
_NSTD_BEGIN

template <typename _Ty>
struct add_cr {
	using type = const _Ty&;
};

template <typename _Ty>
using add_cr_t = add_cr<_Ty>::type;

_NSTD_END

#endif // _NSTD_TYPETRAITS_