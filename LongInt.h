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

	template <typename size_type>
	LongInt(size_type count) : _Mycont(0, 0) {
		add(count);
	}

	template <typename size_type>
	operator size_type() {
		_NSTD_ASSERT(_STD is_arithmetic_t<size_type>, "Converting LongInt type to non-arithmetic, untemplated type");
		// sizeof(size_type) < _Mysize() = truncation
		return *this | size_type(0);
	}

	uint size() {
		return _Mysize();
	}
	const uint size() const {
		return _Mysize();
	}

	template <typename size_type>
	LongInt& add(size_type count) {
		if (_STD is_integral_v<size_type>) {
			return add(count, sizeof(size_type));
		} else {
			return add(count, count.size());
		}
	}

	template <typename size_type>
	LongInt& add(size_type num, const uint num_bytes) {
		if (_Mysize() < num_bytes)
			_Grow(num_bytes - _Mysize());
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

private:
	void _Grow(uint grow_by) {
		uchar* cashe = _Myarr();
		uint nsz(_Mysize() + grow_by);
		_Myarr() = new uchar[nsz](0);
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] = cashe[_I];
		_Mysize() = nsz;
		delete[] cashe;
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