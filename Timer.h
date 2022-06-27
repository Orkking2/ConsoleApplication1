#pragma once
#ifndef _NSTD_TIMER_
#define _NSTD_TIMER_

#include <unordered_map>
#include <chrono>
#include <vector>
#include "Defines.h"

_NSTD_BEGIN

class Timer {
	typedef _NSTD uint uint;

private:
	_STD chrono::steady_clock::time_point epoch_;
	_STD vector<uint> ids_;
	_STD unordered_map<uint, _STD chrono::steady_clock::time_point> id_map_;

public:
	Timer(_STD chrono::steady_clock::time_point epoch = _STD chrono::steady_clock::now()) : epoch_(epoch) {}
	void set(_STD chrono::steady_clock::time_point t) { set(0Ui64, t); }
	void set(uint id = 0Ui64, _STD chrono::steady_clock::time_point t = _STD chrono::steady_clock::now()) {
		id = gen_id(id);
		
	}
	// Progressive passive generation, i.e. 1 -> 2 -> 3
	_NODISCARD uint gen_id(uint id_base = 1Ui64) {
		return id_base ? is_id(id_base) ? gen_id(++id_base) : id_base : gen_id(++id_base);
	}
	_NODISCARD bool is_id(uint id) {
		_NSTD_FOR_I(ids_.size())
			if (ids_[i] == id)
				return true;
		return false;
	}
};




_NSTD_END
#endif // _NSTD_TIMER_