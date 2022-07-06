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
	operator size_type() {
		_NSTD_ASSERT(_STD is_integral_v<size_type>, "Converting LongInt type to non-arithmetic type");
		size_type out(0);
		_NSTD_FOR_I(_Mysize())
			(out += _Myarr()[_Mysize() - (_I + 1)]) <<= CHAR_BIT;
		return out;
	}
	explicit operator bool() {
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
		if (_STD is_integral_v<size_type>) {
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
			cashe_arr = uchar(0) | sum;
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
	LongInt& multiply(size_type count) {
		return multiply(count, deduce_size(count));
	}
	template <typename size_type>
	LongInt& multiply(size_type count, const uint num_bytes) {
		_Grow_if(num_bytes);
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




private:
	void _Grow_if(const uint& new_size) {
		if (_Mysize() < new_size)
			_Grow_to(new_size);
	}
	void _Grow_to(const uint& new_size) {
		uchar* cashe = _Myarr();
		uint nsz(new_size);
		_Myarr() = new uchar[nsz](0);
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] = cashe[_I];
		_Mysize() = nsz;
		delete[] cashe;
	}

	void _Set_to(const LongInt& other) {
		_Grow_if(other._Mysize());
		_NSTD_FOR_I(other._Mysize())
			_Myarr()[_I] = other._Myarr()[_I];
		if (_Mysize() > other._Mysize())
			_NSTD_FOR_I(_Mysize() - other._Mysize())
				_Myarr()[_I + other._Mysize()] = uchar(0);
	}

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


_NSTD_END
#endif