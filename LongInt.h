#pragma once
#ifndef _NSTD_LONGINT_
#define _NSTD_LONGINT_

#include <type_traits>
#include "Defines.h"
#include "Pair.h"

_NSTD_BEGIN

class LongInt {
	typedef unsigned long long uint;
	typedef unsigned char      uchar;

	static constexpr uint _Maxsize = 256Ui64;

	_NSTD pair<uint, uchar*> _Mycont;

public:
	LongInt() : _Mycont(0, 0) {}

	LongInt(const LongInt& other) : _Mycont(0, 0) {
		_Set_to(other);
	}

	template <typename size_type>
	LongInt(size_type count) : _Mycont(0, 0) {
		add(count);
	}

	~LongInt() { delete[] _Myarr(); }

	template <typename size_type>
	LongInt& grow(size_type new_size) {
		_Grow_if(new_size);
		return *this;
	}

	template <typename size_type>
	operator size_type() {
		_NSTD_ASSERT(_STD is_integral_v<size_type>, "Converting LongInt type to non-integral type");
		size_type out(0);
		_NSTD_FOR_I_REVERSE(_Mysize()) {
			out += _Myarr()[_I];
			if (_I)
				out <<= CHAR_BIT;
		}
		return out;
	}
	template <typename size_type>
	operator const size_type() const {
		_NSTD_ASSERT(_STD is_integral_v<size_type>, "Converting LongInt type to non-integral type");
		size_type out(0);
		_NSTD_FOR_I_REVERSE(_Mysize()) {
			out += _Myarr()[_I];
			if (_I)
				out <<= CHAR_BIT;
		}
		return out;
	}

	explicit operator bool() {
		_NSTD_FOR_I(_Mysize())
			if (_Myarr()[_I])
				return true;
		return false;
	}
	explicit operator const bool() const {
		_NSTD_FOR_I(_Mysize())
			if (_Myarr()[_I])
				return true;
		return false;
	}

	uint size() {
		return _Mysize();
	}
	const uint size() const {
		return _Mysize();
	}

	template <typename size_type>
	uint deduce_size(size_type num) {
		if constexpr (_STD is_integral_v<size_type>) {
			return sizeof(size_type);
		} else {
			return num.size();
		}
	}

	template <typename size_type>
	LongInt& add(size_type count) {
		return add(count, deduce_size(count));
	}
	template <typename size_type>
	LongInt& add(size_type num, const uint num_bytes) {
		_Grow_if(num_bytes);
		bool overflow(false);
		_NSTD_FOR_I(_Mysize()) {
			uchar& cashe_arr(_Myarr()[_I]);
			uchar  cashe_num(uchar(0) | (num += overflow));
			uint sum(uint(0) + cashe_arr + cashe_num);
			cashe_arr = static_cast<uchar>(sum);
			overflow = sum & _GET_BIT(uint, CHAR_BIT);
			num >>= CHAR_BIT;
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator+= (size_type count) {
		return add(count);
	}
	template <typename size_type>
	LongInt operator+ (size_type count) {
		return LongInt(*this).add(count);
	}

	template <typename size_type>
	LongInt& subtract(size_type count) {
		while (count) {
			size_type b(this->operator~() & count);
			this->operator^= (count);
			count = b << 1;
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator-= (size_type count) {
		return subtract(count);
	}
	template <typename size_type>
	LongInt operator- (size_type count) {
		return LongInt(*this).subtract(count);
	}
	// LongInt is unsigned
	LongInt operator- () = delete;

	template <typename size_type>
	LongInt& multiply(size_type count) {
		LongInt cashe(*this);
		while (count) {
			if (count & uchar(1))
				this->operator+=(cashe);
			cashe <<= 1;
			count >>= 1;
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator*= (size_type count) {
		return multiply(count);
	}
	template <typename size_type>
	LongInt operator* (size_type count) {
		return LongInt(*this).multiply(count);
	}


	enum SHIFT_DIRECTION { LEFT = 0, RIGHT };
	template <typename size_type>
	LongInt& shift(size_type count, SHIFT_DIRECTION dir) {
		if (!count) {
			return *this;
		} else if(count < 0) {
			count -= count << 1;
			dir = dir == RIGHT ? LEFT : RIGHT;
		} else {
			_Grow_if((count + _Myhighest()) / CHAR_BIT + 1);
		}
		LongInt cashe(*this);
		_NSTD_FOR_I(_Mysize()) {
			uchar lOrderP(count % CHAR_BIT);
			if (dir == RIGHT) {
				uint hOrderP(_I + count / CHAR_BIT);
				_Myarr()[_I] = 
					(hOrderP     < _Mysize() ? cashe._Myarr()[hOrderP]     >> lOrderP              : 0) /* right */ |
					(hOrderP + 1 < _Mysize() ? cashe._Myarr()[hOrderP + 1] << (CHAR_BIT - lOrderP) : 0) /* left  */;
			} else if(count / CHAR_BIT <= _I) {
				uint hOrderP(_I - count / CHAR_BIT);
				_Myarr()[_I] = 
					(          cashe._Myarr()[hOrderP]     << lOrderP)                  /* left  */ |
					(hOrderP ? cashe._Myarr()[hOrderP - 1] >> (CHAR_BIT - lOrderP) : 0) /* right */;
			} else {
				_Myarr()[_I] = 0;
			}
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator<<= (size_type count) {
		return shift(count, LEFT);
	}
	template <typename size_type>
	LongInt& operator<< (size_type count) {
		return LongInt(*this).shift(count, LEFT);
	}
	template <typename size_type>
	LongInt& operator>>= (size_type count) {
		return shift(count, RIGHT);
	}
	template <typename size_type>
	LongInt& operator>> (size_type count) {
		return LongInt(*this).shift(count, RIGHT);
	}

	// Logical operators vvv
	
	LongInt operator~ () {
		LongInt cashe(*this);
		_NSTD_FOR_I(_Mysize())
			cashe._Myarr()[_I] = ~_Myarr()[_I];
		return cashe;
	}
	LongInt& operator-- () {
		return subtract(1Ui64);
	}
	LongInt& operator++ () {
		return add(1Ui64);
	}

	template <typename size_type>
	LongInt& operator^= (size_type count) {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] ^= count << _I * CHAR_BIT;
		return *this;
	}
	template <typename size_type>
	LongInt operator^ (size_type count) {
		return LongInt(*this).operator^= (count);
	}

	template <typename size_type>
	LongInt& operator&= (size_type count) {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] &= count << _I * CHAR_BIT;
		return *this;
	}
	template <typename size_type>
	LongInt operator& (size_type count) {
		return LongInt(*this).operator&= (count);
	}



private:
	template <typename size_type>
	void _Grow_if(const size_type& new_size) {
		if (_Mysize() < new_size)
			_Grow_to(new_size);
	}
	template <typename size_type>
	void _Grow_to(const size_type& new_size) {
		_NSTD_ASSERT(new_size < _Maxsize, "LongInt grown to size above _Maxsize");
		uchar* cashe = _Myarr();
		uint nsz(new_size);
		_Myarr() = new uchar[nsz](0);
		_NSTD_FOR_I((_Mysize() < nsz ? _Mysize() : nsz))
			_Myarr()[_I] = cashe[_I];
		_Mysize() = nsz;
		delete[] cashe;
	}
	void _Set_zero() {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] = uchar(0);
	}

	const uint _Myhighest() {
		for (uint _I = _Mysize(); _I; _I--)
			if (_Myarr()[_I - 1])
				for (uint _J = CHAR_BIT; _J; _J--)
					if (_Myarr()[_I - 1] & _GET_BIT(uchar, _J - 1))
						return (_I - 1) * CHAR_BIT + (_J - 1);
		return 0;
	}

	void _Set_to(const LongInt& other) {
		_Grow_if(other._Mysize());
		_NSTD_FOR_I(other._Mysize())
			_Myarr()[_I] = other._Myarr()[_I];
		if (_Mysize() > other._Mysize())
			_NSTD_FOR_I(_Mysize() - other._Mysize())
				_Myarr()[_I + other._Mysize()] = uchar(0);
	}

	//friend _STD iostream& operator<< (_STD iostream&, const LongInt&);

	uint& _Mysize() {
		return _Mycont.first;
	}
	const uint& _Mysize() const {
		return _Mycont.first;
	}

	uchar*& _Myarr() {
		return _Mycont.second;
	}
	const uchar* _Myarr() const {
		return _Mycont.second;
	}
};

/*
_STD iostream& operator<< (_STD iostream& os, const LongInt& li) {
	if (li._Mysize() < sizeof(uint)) {
		return os << static_cast<uint>(li);
	}
}*/

_NSTD_END
#endif