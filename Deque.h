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
	uint arr_sz_, real_sz_, Hpos_, Tpos_, pos_;
public:
	deque(uint size = 10) : bitX_(new uchar[CHAR_BIT](1)), carr_(new uchar[size]), arr_sz_(size), real_sz_(0), Hpos_(0), Tpos_(0), pos_(0) {
		_NSTD_FOR_I(CHAR_BIT - 1)
			bitX_[i + 1] = 2 * bitX_[i];
	}
	~deque() {
		delete[] bitX_;
	}
	bool front() {
		_NSTD_ASSERT(real_sz_, "attempting to pop val of empty deque");
		return carr_[pos_] & bitX_[Hpos_];
	}
	bool pop_front() {
		bool out(front());
		if (--Hpos_ < 0) {
			Hpos_ = 0;
			pos_++;
		}
		return out;
	}
	bool back() {
		_NSTD_ASSERT(real_sz_, "attempting to pop val of empty deque");
		return carr_[pos_ + real_sz_ - 1] & bitX_[Tpos_];
	}
	bool pop_back() {
		bool out(back());
		if (--Tpos_ < 0) {
			Tpos_ = 0;
			real_sz_--;
		}
		return out;
	}
	deque& push_back(bool b) {
		if (++Tpos_ >= CHAR_BIT) {
			Tpos_ = 0;
			if (++real_sz_ + pos_ > arr_sz_) {
				uchar* cashe(carr_);
				carr_ = new uchar[(real_sz_ + pos_) * 2](0);
				_NSTD_FOR_I(arr_sz_)
					carr_[i] = cashe[i];
				arr_sz_ = (real_sz_ + pos_) * 2;
				delete[] cashe;
			}
		}
		if (b)
			carr_[pos_ + real_sz_ - 1] |= bitX_[Tpos_];
		else
			carr_[pos_ + real_sz_ - 1] &= ~bitX_[Tpos_];
		return *this;
	}

	bool operator[] (uint pos) {
		uint real_pos(pos_ + static_cast<uint> (pos / CHAR_BIT));
		_NSTD_ASSERT(real_pos < real_sz_, "tried to access deque element outside deque bounds");
		return carr_[real_pos] & bitX_[pos % CHAR_BIT];
	}

	// Missing deque& push_front(bool) because I'm lazy
};



_NSTD_END
#endif // !_NSTD_DEQUE_