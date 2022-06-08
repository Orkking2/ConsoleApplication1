#pragma once
#ifndef _NSTD_DEQUE_
#define _NSTD_DEQUE_

#include <deque>
#include "Defines.h"

_NSTD_BEGIN

template <typename _Ty>
class deque : _STD deque<_Ty> {};



// This is a class optimized specifically for use in huffman encoding
// See: Huff.h & Huff.cpp
template <>
class deque<bool> {
	typedef _NSTD uint  uint;
	typedef _NSTD uchar uchar;

	uchar* carr_, * bitX_;
	uint arr_sz_, Hpos_, Tpos_;
public:
	deque(uint size = 10) : bitX_(gen_bitX(1)), carr_(new uchar[size]), arr_sz_(size), Hpos_(1), Tpos_(0) {}
	deque(_NSTD deque<bool>& other) : bitX_(gen_bitX(1)), carr_(new uchar[other.arr_sz_]), arr_sz_(other.arr_sz_), Hpos_(other.Hpos_), Tpos_(other.Tpos_) {
		_NSTD_FOR_I(arr_sz_)
			carr_[i] = other.carr_[i];
	}
	~deque() { delete[] bitX_, carr_; }

	// This is a utility.
	// Note: the caller must call delete[] on generated array themselves
	_NODISCARD uchar*& gen_bitX(uint num_bytes) {
		uchar* nbitX(new uchar[num_bytes * CHAR_BIT](1));
		_NSTD_FOR_I(num_bytes * CHAR_BIT - 1) {
			nbitX[i + 1] = 2 * nbitX[i];
		}
		return nbitX;
	}
	
	// Debugging
	uchar*& get_carr_() {
		return carr_;
	}
	uchar*& get_bitX_() {
		return bitX_;
	}
	const uint& get_Hpos_() {
		return Hpos_;
	}
	const uint& get_Tpos_() {
		return Tpos_;
	}

	_NODISCARD uint size() {
		return Tpos_ - Hpos_ + 1;
	}
	_NODISCARD uint real_size(){
		return arr_sz_;
	}
	_NODISCARD bool empty() {
		return size() == 0;
	}
	// Maintains size
	void clear() {
		Tpos_ = 0;
		Hpos_ = 1;
	}
	// Copies contents of old deque into new deque (maintains position)
	void resize(uint num_bools, bool init = false) {
		uint real_sz = static_cast<uint> (num_bools / CHAR_BIT) + 1;
		uchar* cashe(carr_);
		init ? carr_ = new uchar[real_sz](0xff) : carr_ = new uchar[real_sz](0x00);
		if (real_sz >= arr_sz_) {
			_NSTD_FOR_I(arr_sz_)
				carr_[i] = cashe[i];
		} else /* truncation */ {
			_NSTD_FOR_I(real_sz)
				carr_[i] = cashe[i];
			Tpos_ = num_bools + 1;
			if (Hpos_ > Tpos_) 
				clear();
		}
		delete[] cashe;
	}
	void resize_NOCOPY(uint num_bools, bool init = false) {
		delete[] carr_;
		init ? carr_ = new uchar[num_bools](0xff) : carr_ = new uchar[num_bools](0x00);
		clear();
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
	// Literally shrink_front().shrink_back();
	deque& shrink_fit() {
		return shrink_front().shrink_back();
	}
	deque& shrink_front() {
		if (static_cast<uint> (Hpos_ / CHAR_BIT)) {
			uint high_pos(static_cast<uint> (Hpos_ / CHAR_BIT));
			arr_sz_ -= high_pos;
			uchar* narr(new uchar[arr_sz_]);
			_NSTD_FOR_I(arr_sz_)
				narr[i] = carr_[high_pos + i];
			delete[] carr_;
			carr_ = narr;
		}
		return *this;
	}
	deque& shrink_back() {
		if (static_cast<uint> (Tpos_ / CHAR_BIT + 1) < arr_sz_) {
			arr_sz_ = static_cast<uint> (Tpos_ / CHAR_BIT + 1);
			uchar* narr(new uchar[arr_sz_]);
			_NSTD_FOR_I(arr_sz_)
				narr[i] = carr_[i];
			delete[] carr_;
			carr_ = narr;
		}
		return *this;
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

	// at(pos)
	bool operator[] (uint pos) {
		return at(pos);
	}
	// at(pos) only retrieves the val at pos, it does not allow one to change pos (for that use set(pos, bool))
	bool at(uint pos) {
		uint real_pos(pos + Hpos_);
		_NSTD_ASSERT(real_pos <= Tpos_, "tried to access deque element outside deque bounds");
		return carr_[static_cast<uint> (real_pos / CHAR_BIT)] & bitX_[real_pos % CHAR_BIT];
	}
	// set(pos, !at(pos))
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