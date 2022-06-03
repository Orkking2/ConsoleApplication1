#pragma once
#ifndef _NSTD_DEQUE_
#define _NSTD_DEQUE_

#include <utility>
#include <deque>
#include "Defines.h"

_NSTD_BEGIN

template <typename _Ty>
class deque : _STD deque<_Ty> {};



// This is a class optimized specifically for use in huffman encoding
// See: Huff.h & Huff.cpp
template <>
class deque<bool> {
	typedef nstd::uint  uint;
	typedef unsigned char uchar;

	uchar* carr_, * bitX_;
	uint arr_sz_, Hpos_, Tpos_;
public:
	deque(uint size = 10) : bitX_(new uchar[CHAR_BIT](1)), carr_(new uchar[size]), arr_sz_(size), Hpos_(1), Tpos_(0) {
		_NSTD_FOR_I(CHAR_BIT - 1)
			bitX_[i + 1] = 2 * bitX_[i];
	}
	deque(_NSTD deque<bool>& other) : bitX_(new uchar[CHAR_BIT](1)), carr_(new uchar[other.arr_sz_]), arr_sz_(other.arr_sz_), Hpos_(other.Hpos_), Tpos_(other.Tpos_) {
		_NSTD_FOR_I(CHAR_BIT - 1)
			bitX_[i + 1] = 2 * bitX_[i];
		_NSTD_FOR_I(arr_sz_)
			carr_[i] = other.carr_[i];
	}
	~deque() { delete[] bitX_, carr_; }
	
	// Debugging
/*
	uchar*& get_carr_() {
		return carr_;
	}
	const uint& get_arr_sz_() {
		return arr_sz_;
	}
	const uint& get_Hpos_() {
		return Hpos_;
	}
	const uint& get_Tpos_() {
		return Tpos_;
	}
*/

	_NODISCARD uint size() {
		return Tpos_ - Hpos_ + 1;
	}
	_NODISCARD bool empty() {
		return size() == 0;
	}
	void resize(uint size, bool init = false) {
		uchar* cashe(carr_);
		init ?
			carr_ = new uchar[size](11111111): 
			carr_ = new uchar[size](00000000);
		if (size >= arr_sz_) {
			_NSTD_FOR_I(arr_sz_)
				carr_[i] = cashe[i];
		}
		else /* truncation */ {
			_NSTD_FOR_I(size)
				carr_[i] = cashe[i];
		}
		delete[] cashe;
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
	deque& shrink_front() {
		if (static_cast<uint> (Hpos_ / CHAR_BIT)) {

		}
	}
	deque& push_back(bool b) {
		if (static_cast<uint> (++Tpos_ / CHAR_BIT) >= arr_sz_) {
			uchar* cashe(carr_);
			uint nsz(static_cast<uint> (Tpos_ / CHAR_BIT) * 2);
			carr_ = new uchar[nsz](0);
			_NSTD_FOR_I(arr_sz_)
				carr_[i] = cashe[i];
			arr_sz_ = nsz;
			delete[] cashe;
		}
		set(Tpos_ - Hpos_, b);
		return *this;
	}
	// Missing deque& push_front(bool) because I'm lazy & it serves no purpose for Huff.h

	// Note that this does not allow one to change the value of a bit at pos, it only retrieves the stored val at pos
	bool operator[] (uint pos) {
		return at(pos);
	}
	bool at(uint pos) {
		uint real_pos(pos + Hpos_);
		_NSTD_ASSERT(real_pos <= Tpos_, "tried to access deque element outside deque bounds");
		return carr_[static_cast<uint> (real_pos / CHAR_BIT)] & bitX_[real_pos % CHAR_BIT];
	}

	deque& flip(uint pos) {
		return set(pos, !at(pos));
	}
	deque& set(uint pos, bool val = true) {
		uint real_pos(pos + Hpos_);
		_NSTD_ASSERT(real_pos <= Tpos_, "tried to access deque element outside deque bounds");
		if (val)
			carr_[static_cast<uint> (real_pos / CHAR_BIT)] |=  bitX_[real_pos % CHAR_BIT];
		else
			carr_[static_cast<uint> (real_pos / CHAR_BIT)] &= ~bitX_[real_pos % CHAR_BIT];
		return *this;
	}

};

_NSTD_END
#endif // !_NSTD_DEQUE_