#pragma once
#ifndef _NSTD_PAIR_
#define _NSTD_PAIR_

#include "Defines.h"

_NSTD_BEGIN

template <typename _Ty1, typename _Ty2>
class pair {

public:
	using first_type  = _Ty1;
	using second_type = _Ty2;


public:
	pair(_Ty1 first, _Ty2 second) : first(first), second(second) {}
	
	_Ty1 first;
	_Ty2 second;
};


_NSTD_END
#endif