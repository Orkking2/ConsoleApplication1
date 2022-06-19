#pragma once
#ifndef _NSTD_TIMER_
#define _NSTD_TIMER_

#include <chrono>
#include "Defines.h"

_NSTD_BEGIN

class Timer {
	typedef _NSTD uint uint;

public:
	class timer_instance_ {
		_STD chrono::steady_clock::time_point begin_;
		_STD chrono::duration<double, std::milli>* dPtr_;
	public:
		timer_instance_(_STD chrono::duration<double, std::milli>* dPtr) : begin_(_STD chrono::steady_clock::now()), dPtr_(dPtr) {}
		~timer_instance_() { *dPtr_ = _STD chrono::duration<double, std::milli>(_STD chrono::steady_clock::now() - begin_); }
	};

private:
	_STD chrono::steady_clock::time_point epoch_;
	timer_instance_* timers_;
	uint num_timers_;

public:
	Timer(_STD chrono::steady_clock::time_point epoch = _STD chrono::steady_clock::now(), uint num_timers = 0) : epoch_(epoch), timers_(NULL), num_timers_(num_timers) {
		
	}
};




_NSTD_END
#endif // _NSTD_TIMER_