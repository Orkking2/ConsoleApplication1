#pragma once
#ifndef _NSTD_LONGINT_
#define _NSTD_LONGINT_

#include <type_traits>
#include "Defines.h"
#include "Pair.h"

_NSTD_BEGIN

template <typename _Sty = uchar, typename _Alloc = _STD allocator<_Sty>>
class LongInt {
	typedef unsigned long long uint;
	typedef unsigned char      uchar;

#ifdef _NSTD_LONGINT_DEBUGGING_
public:
#endif
	using _Alty        = _STD _Rebind_alloc_t<_Alloc, _Sty>;
	using _Alty_traits = _STD allocator_traits<_Alty>;
	using _Mysize_t    = typename _Alty::size_type;
	using _Mystorage_t = typename _Alty_traits::value_type;
	using _Myptr_t     = typename _Alty_traits::pointer;

	static constexpr _Mysize_t _Mybitsize = sizeof(_Mystorage_t) * CHAR_BIT;

public:
	using _Mypair_t = _NSTD pair<_Mysize_t, _Myptr_t>;

	static constexpr _Mysize_t _Maxsize = 256;

public:
	using allocator_type = _Alloc;
	using storage_type   = _Mystorage_t;
	using size_type      = _Mysize_t;
	using pointer_type   = _Myptr_t;

public:
	LongInt()                : _Mypair(_Gen_basic()) {}
	LongInt(LongInt& other)  : _Mypair(_Gen_basic()) { _Set_to(other); }
	template <typename size_type>
	LongInt(size_type count) : _Mypair(_Gen_basic()) { add(count);     }

	~LongInt() { _Tidy(); }

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
				out <<= _Mybitsize;
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
				out <<= _Mybitsize;
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

	_Mysize_t size() {
		return _Mysize();
	}
	const _Mysize_t size() const {
		return _Mysize();
	}

	template <typename size_type>
	static uint deduce_size(const size_type& num) {
		if constexpr (_STD is_integral_v<size_type>) {
			return sizeof(size_type);
		} else {
			return num.size();
		}
	}

	template <typename size_type>
	LongInt& add(size_type num) {
		_Grow_if(_Myhighest() / _Mybitsize + _Highest(num) / _Mybitsize + 1);
		bool overflow(false);
		_NSTD_FOR_I(_Mysize()) {
			_Mystorage_t cashe_num(num += overflow);
			overflow = _Overflows(num, *this);
			_Myarr()[_I] += cashe_num;
			_Grow_if(_I + 1 /* _I inclusive */ + overflow);
			num >>= _Mybitsize;
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator+= (const size_type& count) {
		return add(count);
	}
	template <typename size_type>
	LongInt operator+ (const size_type& count) {
		return LongInt(*this).add(count);
	}

	template <typename size_type>
	LongInt& subtract(size_type count) {
		_NSTD_ASSERT(*this >= count, "LongInt is unsigned -- subtracting number larger than LongInt");
		while (count) {
			size_type b(this->operator~() & count);
			this->operator^= (count);
			count = b << 1;
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator-= (const size_type& count) {
		return subtract(count);
	}
	template <typename size_type>
	LongInt operator- (const size_type& count) {
		return LongInt(*this).subtract(count);
	}
	// LongInt is unsigned
	LongInt operator- () = delete;

	template <typename size_type>
	LongInt& multiply(size_type count) {
		LongInt cashe(*this);
		while (count) {
			if (count & 1)
				this->operator+=(cashe);
			cashe <<= 1;
			count >>= 1;
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator*= (const size_type& count) {
		return multiply(count);
	}
	template <typename size_type>
	LongInt operator* (const size_type& count) {
		return LongInt(*this).multiply(count);
	}

	template <typename size_type>
	LongInt divide(size_type divisor, const size_type& persistent_divisor) {
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
		this->operator-= (divisor);
		return quotient + divide(persistent_divisor, persistent_divisor);
	}
	template <typename size_type>
	LongInt& operator/= (const size_type& divisor) {
		_Set_to(divide(divisor, divisor));
		return *this;
	}
	template <typename size_type>
	LongInt operator/ (const size_type& divisor) {
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
			_Grow_if((count + _Myhighest()) / _Mybitsize + 1);
		}
		LongInt cashe(*this);
		_NSTD_FOR_I(_Mysize()) {
			_Mystorage_t lOrderP(count % _Mybitsize);
			if (dir == RIGHT) {
				uint hOrderP(_I + count / _Mybitsize);
				_Myarr()[_I] = 
					(hOrderP     < _Mysize() ? cashe._Myarr()[hOrderP]     >> lOrderP              : 0) /* right */ |
					(hOrderP + 1 < _Mysize() ? cashe._Myarr()[hOrderP + 1] << (_Mybitsize - lOrderP) : 0) /* left  */;
			} else if(count / _Mybitsize <= _I) {
				uint hOrderP(_I - count / _Mybitsize);
				_Myarr()[_I] = 
					(          cashe._Myarr()[hOrderP]     << lOrderP)                  /* left  */ |
					(hOrderP ? cashe._Myarr()[hOrderP - 1] >> (_Mybitsize - lOrderP) : 0) /* right */;
			} else {
				_Myarr()[_I] = 0;
			}
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator<<= (const size_type& count) {
		return shift(count, LEFT);
	}
	template <typename size_type>
	LongInt& operator<< (const size_type& count) {
		return LongInt(*this).shift(count, LEFT);
	}
	template <typename size_type>
	LongInt& operator>>= (const size_type& count) {
		return shift(count, RIGHT);
	}
	template <typename size_type>
	LongInt& operator>> (const size_type& count) {
		return LongInt(*this).shift(count, RIGHT);
	}

	// Logical operators vvv
	template <typename size_type>
	LongInt& operator= (const size_type& other) {
		_Set_zero();
		return add(other);
	}

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
	LongInt& operator^= (const size_type& count) {
		_NSTD_FOR_I(_Mysize()) 
			_Myarr()[_I] ^= count >> _I * _Mybitsize;
		return *this;
	}
	template <typename size_type>
	LongInt operator^ (const size_type& count) {
		return LongInt(*this).operator^= (count);
	}

	template <typename size_type>
	LongInt& operator&= (const size_type& count) {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] &= count >> _I * _Mybitsize;
		return *this;
	}
	template <typename size_type>
	LongInt operator& (const size_type& count) {
		return LongInt(*this).operator&= (count);
	}

	template <typename size_type>
	LongInt& operator|= (const size_type& count) {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] |= count >> _I * _Mybitsize;
		return *this;
	}
	template <typename size_type>
	LongInt operator| (const size_type& count) {
		return LongInt(*this).operator|= (count);
	}

	// Comparisons
	template <typename size_type>
	bool operator!= (const size_type& count) {
		_NSTD_FOR_I_REVERSE(_Mysize())
			if (_Myarr()[_I] != count >> _I * _Mybitsize)
				return true;
		return false;
	}
	template <typename size_type>
	bool operator== (const size_type& count) {
		return !this->operator!= (count);
	}

	template <typename size_type>
	bool operator> (const size_type& count) {
		if (_Mysize() * _Mybitsize % 64 /* word size */ != _Mysize() * _Mybitsize) {
			// int >> count == int >> (count % word size) <- avoid
			size_type ccashe(count);
			_NSTD_FOR_I(_Mysize() * _Mybitsize / 64)
				(ccashe >>= 63) >>= 1;
			ccashe >>= _Mysize() * _Mybitsize % 64;
			if (ccashe)
				return false;
		} else if (count >> _Mysize() * _Mybitsize) {
			return false;
		}
		_NSTD_FOR_I_REVERSE(_Mysize()) 
			if (_Myarr()[_I] > count >> _I * _Mybitsize) 
				return true;
		return false;
	}
	template <typename size_type>
	bool operator>= (const size_type& count) {
		return this->operator> (count) || this->operator== (count);
	}
	template <typename size_type>
	bool operator< (const size_type& count) {
		return !this->operator>= (count);
	}
	template <typename size_type>
	bool operator<= (const size_type& count) {
		return !this->operator> (count);
	}

#ifdef _COMPARE_
	template <typename size_type>
	_STD strong_ordering operator<=> (const size_type& count) {
		if (this->operator> (count))
			return _STD strong_ordering::greater;
		else if (this->operator== (count))
			return _STD strong_ordering::equal;
		else
			return _STD strong_ordering::less;
	}
#endif

private:
#ifdef _NSTD_LONGINT_DEBUGGING_
public:
#endif
	_NODISCARD static _Mypair_t _Gen_basic() {
		_Alty alloc;
		_Myptr_t p(alloc.allocate(1));
		_Alty_traits::construct(alloc, p, 0);
		return _Mypair_t(1, p);
	}

	template <typename size_type>
	void _Grow_if(const size_type& new_size) {
		if (_Mysize() < new_size)
			_Grow_to(new_size);
	}
	template <typename size_type>
	void _Grow_to(const size_type& new_size) {
		_NSTD_ASSERT(new_size > 0, "Cannot set LongInt size to or below 0");
		_NSTD_ASSERT(new_size <= _Maxsize, "LongInt grown to size above _Maxsize");
		
		_Alty alloc;
		LongInt cashe(*this);
		_Myarr() = alloc.allocate(new_size);
		_NSTD_ASSERT(_Myarr(), "Failed to allocate memory");
		_NSTD_FOR_I(new_size)
			_Alty_traits::construct(alloc, (_Myarr() + _I), 0);
		_NSTD_FOR_I((_Mysize() < new_size ? _Mysize() : new_size))
			_Myarr()[_I] = cashe._Myarr()[_I];
		_Mysize() = new_size;
	}
	
	void _Set_zero() {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] = storage_type(0);
	}
	template <typename size_type>
	static const bool _Overflows(const size_type& count, const LongInt& li) {
		if (_HIGH_BIT(_Mystorage_t) & count & li)
			return true;
		else if (_HIGH_BIT(_Mystorage_t) & (count | li))
			return _Overflows(count << 1, li << 1);
		return false;
	}
	template <typename size_type>
	static const uint _Highest(size_type count) {
		if (!count)
			return 0;
		const size_type cashe(count < 0 ? count -= count << 1 : count);
		_NSTD_FOR_I(cashe) {
			if (count >> _Mybitsize) {
				count >>= _Mybitsize;
				continue;
			}
			_NSTD_FOR_J_REVERSE(_Mybitsize)
				if (_GET_BIT(storage_type, _J) & count)
					return _I * _Mybitsize + _J;
		}
		return 0Ui64;
	}
	const _Mysize_t _Myhighest() {
		_NSTD_FOR_I_REVERSE(_Mysize())
			if (_Myarr()[_I])
				_NSTD_FOR_J_REVERSE(_Mybitsize)
					if (_Myarr()[_I] & _GET_BIT(storage_type, _J))
						return _I * _Mybitsize + _J;
		return 0;
	}

	void _Set_to(LongInt& other) {
		_Grow_if(other._Mysize());
		_NSTD_FOR_I(other._Mysize())
			_Myarr()[_I] = other._Myarr()[_I];
		if (_Mysize() > other._Mysize())
			_NSTD_FOR_I(_Mysize() - other._Mysize())
				_Myarr()[_I + (other._Mysize() - 1)] = storage_type(0);
	}

	void _Tidy() {
		_Alty alloc;
		alloc.deallocate(_Myarr(), _Mysize());
	}

	_Mysize_t& _Mysize() {
		return _Mypair.first;
	}
	const _Mysize_t& _Mysize() const {
		return _Mypair.first;
	}

	_Myptr_t& _Myarr() {
		return _Mypair.second;
	}
	const _Myptr_t _Myarr() const {
		return _Mypair.second;
	}

	_Mypair_t _Mypair;
};

_NSTD_END
#endif