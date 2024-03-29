#pragma once
#ifndef _NSTD_PAIR_
#define _NSTD_PAIR_

#include "Defines.h"

_NSTD_BEGIN

template <typename _Ty1, typename _Ty2>
struct pair {
	using first_type  = _Ty1;
	using second_type = _Ty2;

	pair(_Ty1 first, _Ty2 second) : first(first),       second(second) {}
	pair(const pair& other)       : first(other.first), second(other.second) {}
#ifdef _UTILITY_
	pair(const _STD pair<_Ty1, _Ty2>& other) : first(other.first), second(other.second) {}
#endif

	_Ty1 first;
	_Ty2 second;
};


_NSTD_END
#endif