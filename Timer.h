#pragma once
#ifndef _NSTD_TIMER_
#define _NSTD_TIMER_

#include "Defines.h"
#include <chrono>

_NSTD_BEGIN

class timer {
	_STD chrono::time_point<_STD chrono::steady_clock> start_time_;
public:
	timer() : start_time_(_STD chrono::steady_clock::now()) {}
	_STD chrono::duration<_STD chrono::nanoseconds> get() {
		//return _STD chrono::duration_cast<_STD chrono::nanoseconds>(_STD chrono::steady_clock::now() - start_time_);
	}

};

_NSTD_END
#endif // !_NSTD_TIMER_