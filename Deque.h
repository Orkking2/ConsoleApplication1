#pragma once
#ifndef _NSTD_DEQUE_
#define _NSTD_DEQUE_

#include "Defines.h"

_NSTD_BEGIN

template <class _Ty>
class Deque {
public:
	struct LObj {
		LObj(_Ty& val, LObj* l, LObj* r) : val_(val), l_(l), r_(r) {}
		_Ty val_; LObj* l_, * r_;
	};
private:
	LObj* l_, * r_;
public:
	Deque() : l_(NULL), r_(NULL) {}
	
};




_NSTD_END
#endif // !_NSTD_DEQUE_