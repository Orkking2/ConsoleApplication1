#pragma once
#ifndef _NSTD_TIMER_
#define _NSTD_TIMER_

#include <iostream>
#include <chrono>
#include "Defines.h"

_NSTD_BEGIN

class Timer {
	_STD chrono::steady_clock::time_point start_;
public:
	Timer() : start_(_STD chrono::steady_clock::now()) {}
	~Timer() {
		_STD cout << "\nProcess took " << _STD chrono::duration<double, std::milli>(_STD chrono::steady_clock::now() - start_).count() << " milliseconds\n";
	}
};




_NSTD_END
#endif // _NSTD_TIMER_