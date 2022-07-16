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
	LongInt() : _Mycont(_Gen_basic()) {}

	LongInt(const LongInt& other) : _Mycont(_Gen_basic()) {
		_Set_to(other);
	}

	template <typename size_type>
	LongInt(size_type count) : _Mycont(_Gen_basic()) {
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
	LongInt& add(size_type num) {
		_Grow_if(_Myhighest() / CHAR_BIT + _Highest(num) / CHAR_BIT + 1);
		bool overflow(false);
		_NSTD_FOR_I(_Mysize()) {
			uchar  cashe_num(uchar(0) | (num += overflow));
			uint sum(uint(0) + _Myarr()[_I] + cashe_num);
			_Myarr()[_I] = static_cast<uchar>(sum);
			overflow = sum & _GET_BIT(uint, CHAR_BIT);
			_Grow_if(_I + 1 /* _I inclusive */ + overflow);
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

	template <typename size_type>
	LongInt divide(size_type divisor, const size_type& persistent_divisor) {
		_STD cout << static_cast<uint>(*this) << '\n';
		// LongInt is unsigned
		divisor = divisor < 0 ? divisor - (divisor << 1) : divisor;
		LongInt quotient(1Ui64), &dividend(*this);

		if (dividend == divisor) {
			return 1;
		} else if (dividend < divisor) {
			return 0;
		}

		while (dividend >= divisor) {
			divisor <<= 1;
			quotient <<= 1;
		}

		if (dividend < divisor) {
			divisor >>= 1;
			quotient >>= 1;
		}
		dividend -= divisor;
		return quotient + divide(persistent_divisor, persistent_divisor);
	}
	template <typename size_type>
	LongInt& operator/= (size_type divisor) {
		_Set_to(divide(divisor, divisor));
		return *this;
	}
	template <typename size_type>
	LongInt operator/ (size_type divisor) {
		return divide(divisor, divisor);
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
		_NSTD_FOR_I(cashe._Mysize())
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

	template <typename size_type>
	LongInt& operator|= (size_type count) {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] |= count << _I * CHAR_BIT;
		return *this;
	}
	template <typename size_type>
	LongInt operator| (size_type count) {
		return LongInt(*this).operator|= (count);
	}

	// Comparisons
	template <typename size_type>
	bool operator!= (size_type count) {
		_NSTD_FOR_I_REVERSE(_Mysize())
			if (_Myarr()[_I] != count << _I * CHAR_BIT)
				return true;
		return false;
	}
	template <typename size_type>
	bool operator== (size_type count) {
		return !this->operator!= (count);
	}

	template <typename size_type>
	bool operator> (size_type count) {
		if (count >> _Mysize() * CHAR_BIT)
			return false;
		_NSTD_FOR_I_REVERSE(_Mysize()) 
			if (_Myarr()[_I] > (count >> _I * CHAR_BIT)) 
				return true;
		return false;
	}
	template <typename size_type>
	bool operator>= (size_type count) {
		return this->operator> (count) || this->operator== (count);
	}
	template <typename size_type>
	bool operator< (size_type count) {
		return !this->operator>= (count);
	}
	template <typename size_type>
	bool operator<= (size_type count) {
		return !this->operator> (count);
	}

#ifdef _COMPARE_
	template <typename size_type>
	_STD strong_ordering operator<=> (size_type count) {
		if (this->operator> (count))
			return _STD strong_ordering::greater;
		else if (this->operator== (count))
			return _STD strong_ordering::equal;
		else
			return _STD strong_ordering::less;
	}
#endif

private:
	_NSTD pair<uint, uchar*> _Gen_basic() {
		return _NSTD pair<uint, uchar*>(1, new uchar(0));
	}

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
	public:
	template <typename size_type>
	static const uint _Highest(size_type count) {
		const size_type cashe(count < 0 ? count -= count << 1 : count);
		_NSTD_FOR_I(cashe) {
			if (count >> CHAR_BIT) {
				count >>= CHAR_BIT;
				continue;
			}
			_NSTD_FOR_J_REVERSE(CHAR_BIT)
				if (_GET_BIT(uchar, _J) & count)
					return _I * CHAR_BIT + _J;
		}
		return 0Ui64;
	}
	const uint _Myhighest() {
		_NSTD_FOR_I_REVERSE(_Mysize())
			if (_Myarr()[_I])
				_NSTD_FOR_J_REVERSE(CHAR_BIT)
					if (_Myarr()[_I] & _GET_BIT(uchar, _J))
						return _I * CHAR_BIT + _J;
		return 0Ui64;
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