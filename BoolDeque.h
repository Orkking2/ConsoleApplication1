#pragma once
#ifndef _NSTD_DEQUE_
#define _NSTD_DEQUE_

#include <utility>
#include <deque>
#include "Defines.h"

_NSTD_BEGIN

template <typename _Ty>
class deque {

};



template <>
class deque<bool> {
	typedef unsigned int uint;

#define CHAR_BITSIZE 8
	_STD pair<char*, uint> asp_;
	uint pot_sz_, real_sz_;
public:

};



_NSTD_END
#endif // !_NSTD_DEQUE_