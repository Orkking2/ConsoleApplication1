#pragma once
#ifndef _NSTD_DEQUE_
#define _NSTD_DEQUE_

#include <utility>
#include <deque>
#include "Defines.h"

_NSTD_BEGIN

template <typename _Ty>
class deque : _STD deque<_Ty> {

};


// This is a class optimized specifically for use in huffman encoding
// see Huff.h & Huff.cpp
template <>
class deque<bool> {
	typedef unsigned int uint;
	typedef unsigned char uchar;

	uchar* bitX_;

	uchar* carr_;
	uint arr_sz_, Hpos_, Tpos_;
public:
	deque(uint size = 10) : bitX_(new uchar[CHAR_BIT](1)), carr_(new uchar[size]), arr_sz_(size), Hpos_(0), Tpos_(0) {
		_NSTD_FOR_I(CHAR_BIT - 1)
			bitX_[i + 1] = 2 * bitX_[i];
	}
	~deque() {
		delete[] bitX_;
	}
	_NODISCARD bool front() {
		_NSTD_ASSERT(Tpos_ >= Hpos_, "attempting to pop val of empty deque");
		return carr_[static_cast<uint> (Hpos_ / CHAR_BIT)] & bitX_[Hpos_ % CHAR_BIT];
	}
	bool pop_front() {
		bool out(front());
		++Hpos_;
		return out;
	}
	_NODISCARD bool back() {
		_NSTD_ASSERT(Tpos_ >= Hpos_, "attempting to pop val of empty deque");
		return carr_[static_cast<uint> (Tpos_ / CHAR_BIT)] & bitX_[Tpos_ % CHAR_BIT];
	}
	bool pop_back() {
		bool out(back());
		--Tpos_;
		return out;
	}
	deque& push_back(bool b) {
		if (static_cast<uint> (++Tpos_ / CHAR_BIT) > arr_sz_) {
			uchar* cashe(carr_);
			uint nsz(static_cast<uint> (Tpos_ / CHAR_BIT) * 2);
			carr_ = new uchar[nsz](0);
			_NSTD_FOR_I(arr_sz_)
				carr_[i] = cashe[i];
			arr_sz_ = nsz;
			delete[] cashe;
		}
		if (b)
			carr_[static_cast<uint> (Tpos_ / CHAR_BIT)] |=  bitX_[Tpos_ % CHAR_BIT];
		else
			carr_[static_cast<uint> (Tpos_ / CHAR_BIT)] &= ~bitX_[Tpos_ % CHAR_BIT];
		return *this;
	}

	bool operator[] (uint pos) {
		uint real_pos(pos + Hpos_);
		_NSTD_ASSERT(real_pos < Tpos_, "tried to access deque element outside deque bounds");
		return carr_[static_cast<uint> (real_pos / CHAR_BIT)] & bitX_[real_pos % CHAR_BIT];
	}

	// Missing deque& push_front(bool) because I'm lazy
};



_NSTD_END
#endif // !_NSTD_DEQUE_