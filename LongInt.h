#pragma once
#ifndef _NSTD_LONGINT_
#define _NSTD_LONGINT_

#include <xmemory>
#include "Defines.h"
#include "Pair.h"

_NSTD_BEGIN

template <typename _Storage_type = uchar, typename _Alloc = _STD allocator<_Storage_type>>
class LongInt {
#ifdef _NSTD_LONGINT_DEBUGGING_
public:
#endif
	using _Alty        = _STD _Rebind_alloc_t<_Alloc, _Storage_type>;
	using _Alty_traits = _STD allocator_traits<_Alty>;
	using _Mysize_t    = typename _Alty::size_type;
	using _Mystorage_t = typename _Alty_traits::value_type;
	using _Myptr_t     = typename _Alty_traits::pointer;

	static constexpr _Mysize_t _Mybytesize = sizeof(_Mystorage_t);
	static constexpr _Mysize_t _Mybitsize  = _Mybytesize * CHAR_BIT;

public:
	using _Mypair_t = _NSTD pair<_Mysize_t, _Myptr_t>;

	// Max 256 bytes (exclusive)
	static constexpr _Mysize_t _Maxsize = 256 / _Mybytesize;

	// Utility(?)
	static constexpr auto _MAX_OF = []() -> LongInt {
		return ~LongInt().grow(LongInt::_Maxsize - 1);
	};

	enum SHIFT_DIRECTION { LEFT = 0, RIGHT };
	template <typename size_type1, typename size_type2>
	// Explicitly doesn't shift by reference
	static size_type1 _Real_shift(size_type1 num, const size_type2& shift, const SHIFT_DIRECTION& dir) {
		if (dir == RIGHT) {
			_NSTD_FOR_I(shift / 64)
				(num >>= 63) >>= 1;
			num >>= shift % 64;
		} else {
			_NSTD_FOR_I(shift / 64)
				(num <<= 63) <<= 1;
			num <<= shift % 64;
		}
		return num;
	}

	template <typename size_type1, typename size_type2>
	static constexpr auto const& _Min(const size_type1& first, const size_type2& second) {
		return first > second ? second : first;
	}

public:
	using allocator_type = _Alloc;
	using storage_type   = _Mystorage_t;
	using size_type      = _Mysize_t;
	using pointer_type   = _Myptr_t;

public:
	LongInt()                                                     : _Mypair(_Gen_basic()) {}
	LongInt(const LongInt& other)                                 : _Mypair(_Gen_basic()) { _Set_to(other); }
	
	template <typename other_storage_t, typename other_alloc_t>
	LongInt(const LongInt<other_storage_t, other_alloc_t>& other) : _Mypair(_Gen_basic()) { _Set_to(other); }
	
	template <typename size_type>
	LongInt(const size_type& count)                               : _Mypair(_Gen_basic()) { add(count); }

	template <typename size_type>
	LongInt(size_type&& count)									  : _Mypair(_Gen_basic()) { add(count); }

	~LongInt() { _Tidy(); }

	// Grow by (in bytes)
	template <typename size_type>
	LongInt& grow(const size_type& size) {
		_Grow_by(size);
		return *this;
	}

	LongInt& shrink_fit() {
		if (_Myhighest() / _Mybitsize + 1 < _Mysize())
			_Grow_to(_Myhighest() / _Mybitsize + 1);
		return *this;
	}

	template <typename size_type>
	operator size_type() {
		size_type out(0);
		_NSTD_FOR_I_REVERSE(_Mysize()) {
			out += _Myarr()[_I];
			if (_I)
				out = _Real_shift(out, _Mybitsize, LEFT);
		}
		return out;
	}
	template <typename size_type>
	operator const size_type() const {
		size_type out(0);
		_NSTD_FOR_I_REVERSE(_Mysize()) {
			out += _Myarr()[_I];
			if (_I)
				out = _Real_shift(out, _Mybitsize, LEFT);
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

	template <typename other_storage_t, typename other_alloc_t>
	explicit operator LongInt<other_storage_t, other_alloc_t>() {
		return LongInt<other_storage_t, other_alloc_t>(*this);
	}
	template <typename other_storage_t, typename other_alloc_t>
	explicit operator const LongInt<other_storage_t, other_alloc_t>() const {
		return LongInt<other_storage_t, other_alloc_t>(*this);
	}

	_Mysize_t size() {
		return _Mysize();
	}
	const _Mysize_t size() const {
		return _Mysize();
	}

/*	Legacy
*	template <typename size_type>
*	static _Mysize_t deduce_size(const size_type& num) {
*		return sizeof(size_type);
*	}
*	template <typename other_storage_t, typename other_alloc_t>
*	static _Mysize_t deduce_size(const LongInt<other_storage_t, other_alloc_t>& other) {
*		return other.size();
*	}
*/

	template <typename size_type>
	LongInt& add(size_type num) {
		if (!num) 
			return *this;
		_Grow_if((_Myhighest() + _Highest(num)) / _Mybitsize + 1);
		bool overflow(false);
		_NSTD_FOR_I(_Mysize()) {
			_Mystorage_t cache_num(num += (overflow ? 1 : 0));
			overflow = _Overflows(num, _Myarr()[_I]);
			_Myarr()[_I] += cache_num;
			_Grow_if(_I + 1 /* _I inclusive */ + (overflow ? 1 : 0));
			num = _Real_shift(num, _Mybitsize, RIGHT);
		}
		return this->shrink_fit();
	}
	template <typename size_type>
	LongInt& operator+= (const size_type& count) {
		return add(count);
	}
	template <typename size_type>
	LongInt operator+ (const size_type& count) const {
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
	LongInt operator- (const size_type& count) const {
		return LongInt(*this).subtract(count);
	}
	// LongInt is unsigned
	LongInt operator- () = delete;

	template <typename size_type>
	LongInt& multiply(size_type count) {
		_Make_abs(count);
		LongInt cache(*this);
		_Set_zero();
		while (count) {
			if (count & _LOW_BIT(size_type)) 
				this->operator+= (cache);
			cache <<= 1;
			count >>= 1;
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator*= (const size_type& count) {
		return multiply(count);
	}
	template <typename size_type>
	LongInt operator* (const size_type& count) const {
		return LongInt(*this).multiply(count);
	}

	template <typename size_type>
	LongInt& divide(size_type divisor) {
		_Make_abs(divisor);
		LongInt dividend(*this);
		_Set_zero(); // *this is output
		_NSTD_FOR_I_REVERSE(dividend._Myhighest() + 1) {
			if (dividend >= divisor << _I) {
				dividend -= divisor << _I;
				this->operator+= (LongInt(1) <<= _I);
			}
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator/= (const size_type& divisor) {
		return divide(divisor);
	}
	template <typename size_type>
	LongInt operator/ (const size_type& divisor) const {
		return LongInt(*this).divide(divisor);
	}

	template <typename size_type>
	LongInt& mod(const size_type& count) {
		if (_Highest(count) > _Myhighest())
			return *this;

		_NSTD_FOR_I_REVERSE(_Myhighest() + 1)
			if(this->operator> (count << _I))
				this->operator-= (count << _I);

		return *this;
	}
	template <typename size_type>
	LongInt& operator%= (const size_type& count) {
		return mod(count);
	}
	template <typename size_type>
	LongInt operator% (const size_type& count) const {
		return LongInt(*this).mod(count);
	}

	template <typename size_type>
	LongInt& shift(size_type count, SHIFT_DIRECTION dir) {
		if (!count) {
			return *this;
		} else if(count < 0) {
			count -= count << 1;
			dir = dir == RIGHT ? LEFT : RIGHT;
		}
		if (dir == LEFT)
			_Grow_if((_Myhighest() + count) / _Mybitsize + 1);
		LongInt cache(*this);
		cache._Grow_if(_Mysize());
		_Set_zero();
		_NSTD_FOR_I(_Mysize()) {
			_Mystorage_t lOrderP(count % _Mybitsize);
			if (dir == RIGHT) {
				_Mysize_t hOrderP(_I + count / _Mybitsize);
				_Myarr()[_I] = 
					(hOrderP     < _Mysize() ? cache._Myarr()[hOrderP]     >> lOrderP                : 0) /* right */ |
					(hOrderP + 1 < _Mysize() ? cache._Myarr()[hOrderP + 1] << (_Mybitsize - lOrderP) : 0) /* left  */;
			} else if(count / _Mybitsize <= _I) {
				_Mysize_t hOrderP(_I - count / _Mybitsize);
				_Myarr()[_I] = 
					(          cache._Myarr()[hOrderP]     << lOrderP)                    /* left  */ |
					(hOrderP ? cache._Myarr()[hOrderP - 1] >> (_Mybitsize - lOrderP) : 0) /* right */;
			}
		}
		return *this;
	}
	template <typename size_type>
	LongInt& operator<<= (const size_type& count) {
		return shift(count, LEFT);
	}
	template <typename size_type>
	LongInt operator<< (const size_type& count) const {
		return LongInt(*this).shift(count, LEFT);
	}
	template <typename size_type>
	LongInt& operator>>= (const size_type& count) {
		return shift(count, RIGHT);
	}
	template <typename size_type>
	LongInt operator>> (const size_type& count) const {
		return LongInt(*this).shift(count, RIGHT);
	}

	// Logical operators vvv
	template <typename size_type>
	LongInt& operator= (const size_type& other) {
		_Set_zero();
		return add(other);
	}
	template <typename other_size_t, typename other_alloc_t>
	LongInt& operator= (const LongInt<other_size_t, other_alloc_t>& other) {
		_Set_to(other);
		return *this;
	}
	LongInt& operator= (const LongInt& other) {
		if (this != &other)
			_Set_to(other);
		return *this;
	}

	LongInt operator~ () const {
		LongInt cache(*this);
		cache._Grow_if(_Mysize());
		_NSTD_FOR_I(cache._Mysize())
			cache._Myarr()[_I] = ~_Myarr()[_I];
		return cache;
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
	LongInt operator^ (const size_type& count) const {
		return LongInt(*this).operator^= (count);
	}

	template <typename size_type>
	LongInt& operator&= (const size_type& count) {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] &= count >> _I * _Mybitsize;
		return *this;
	}
	template <typename size_type>
	LongInt operator& (const size_type& count) const {
		return LongInt(*this).operator&= (count);
	}

	template <typename size_type>
	LongInt& operator|= (const size_type& count) {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] |= count >> _I * _Mybitsize;
		return *this;
	}
	template <typename size_type>
	LongInt operator| (const size_type& count) const {
		return LongInt(*this).operator|= (count);
	}

	// Comparisons
	template <typename size_type>
	bool operator!= (const size_type& count) const {
		if (_Highest(count) != _Myhighest())
			return true;
		_NSTD_FOR_I_REVERSE(_Mysize())
			if (_Myarr()[_I] != static_cast<_Mystorage_t>(_Real_shift(count, _I * _Mybitsize, RIGHT)))
				return true;
		return false;
	}
	template <typename size_type>
	bool operator== (const size_type& count) const {
		return !this->operator!= (count);
	}

	template <typename size_type>
	bool operator> (const size_type& count) const {
		if (_Highest(count) != _Myhighest())
			return _Myhighest() > _Highest(count);
		
		_NSTD_FOR_I_REVERSE(_Mysize()) {
			_Mystorage_t ccache(_Real_shift(count, _I * _Mybitsize, RIGHT));
			if (_Myarr()[_I] != ccache)
				return _Myarr()[_I] > ccache;
		}
		return false;
	}
	template <typename size_type>
	bool operator>= (const size_type& count) const {
		return this->operator> (count) || this->operator== (count);
	}
	template <typename size_type>
	bool operator< (const size_type& count) const {
		return !this->operator>= (count);
	}
	template <typename size_type>
	bool operator<= (const size_type& count) const {
		return !this->operator> (count);
	}

#ifdef _COMPARE_
	template <typename size_type>
	_STD strong_ordering operator<=> (const size_type& count) const {
		if (this->operator> (count))
			return _STD strong_ordering::greater;
		else if (this->operator== (count))
			return _STD strong_ordering::equal;
		else
			return _STD strong_ordering::less;
	}
#endif

#ifdef _IOSTREAM_
	template <typename storage_t, typename alloc_t>
	friend _STD ostream& operator<< (_STD ostream&, const LongInt<storage_t, alloc_t>&);
#endif

private:
#ifdef _NSTD_LONGINT_DEBUGGING_
public:
#endif
	// Lightweight wrapper
	class _Mypair_wrapper_t {
	public:
		_Mypair_wrapper_t(_Mypair_t p) : _Mypair(p) {}
		~_Mypair_wrapper_t() {
			_Alty alloc;
			alloc.deallocate(_Mypair.second, _Mypair.first);
		}
		operator _Mypair_t() {
			return _Mypair;
		}
	private:
		_Mypair_t _Mypair;
	};

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
	void _Grow_by(const size_type& size) {
		_Grow_to(_Mysize() + size);
	}
	template <typename size_type>
	void _Grow_to(const size_type& new_size) {
		_NSTD_ASSERT(new_size > 0, "Cannot set LongInt size to or below 0");
		_NSTD_ASSERT(new_size <= _Maxsize, "LongInt grown to size above _Maxsize");

		_Alty alloc;
		LongInt cache(*this);
		_Myarr() = alloc.allocate(new_size);
		_NSTD_ASSERT(_Myarr(), "Failed to allocate memory");
		_NSTD_FOR_I(new_size)
			_Alty_traits::construct(alloc, (_Myarr() + _I), 0);
		_NSTD_FOR_I(_Min(cache._Mysize(), new_size))
			_Myarr()[_I] = cache._Myarr()[_I];
		_Mysize() = new_size;
	}
	
	void _Set_zero() {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] = _Mystorage_t(0);
	}

	template <typename size_type1, typename size_type2>
	static const bool _Overflows(size_type1 first, size_type2 second) {
		if (_HIGH_BIT(_Mystorage_t) & first & second) 
			return true;
		else if (_HIGH_BIT(_Mystorage_t) & (first | second)) 
			return _Overflows(first <<= 1, second <<= 1);
		return false;
	}

	template <typename size_type>
	static void _Make_abs(size_type& n) {
		n = n < 0 ? n - (n << 1) : n;
	}
	template <typename size_type>
	static const _Mysize_t _Highest(size_type count) {
		_Make_abs(count);
		if (!count)
			return 0;
		const size_type cache(count);
		_NSTD_FOR_I(cache) {
			if (count >> _Mybitsize) {
				count >>= _Mybitsize;
				continue;
			}
			_NSTD_FOR_J_REVERSE(_Mybitsize)
				if (_GET_BIT(_Mystorage_t, _J) & count)
					return _I * _Mybitsize + _J;
		}
		return 0;
	}
	template <typename other_storage_t, typename other_alloc_t>
	static const _Mysize_t _Highest(const LongInt<other_storage_t, other_alloc_t>& other) {
		_NSTD_FOR_I_REVERSE(other._Mysize())
			if (other._Myarr()[_I])
				_NSTD_FOR_J_REVERSE(other._Mybitsize)
					if (other._Myarr()[_I] & _GET_BIT(other_storage_t, _J))
						return _I * other._Mybitsize + _J;
		return 0;
	}
	const _Mysize_t _Myhighest() const {
		return _Highest(*this);
	}

	template <typename other_storage_t, typename other_alloc_t>
	void _Set_to(const LongInt<other_storage_t, other_alloc_t>& other) {
		_Set_zero();
		_Grow_if(other._Myhighest() / other._Mybitsize + 1);
		_NSTD_FOR_I_REVERSE(other._Mysize()) {
			add(other._Myarr()[_I]);
			if (_I)
				shift(other._Mybitsize, LEFT);
		}
	}
	template <>
	void _Set_to(const LongInt& other) {
		_Set_zero();
		_Grow_if(other._Myhighest() / _Mybitsize + 1);
		_NSTD_FOR_I(_Min(_Mysize(), other._Mysize()))
			_Myarr()[_I] = other._Myarr()[_I];
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
	const _Myptr_t& _Myarr() const {
		return _Mypair.second;
	}

	_Mypair_t _Mypair;
};

#ifdef _IOSTREAM_
template <typename storage_t, typename alloc_t>
_STD ostream& operator<< (_STD ostream& os, const LongInt<storage_t, alloc_t>& li) {
	_NSTD_FOR_I_REVERSE(li.size() * li._Mybitsize) {
		os << (li._Myarr()[_I / li._Mybitsize] & _GET_BIT(storage_t, _I % li._Mybitsize) ? '1' : '0');
		if (!(_I % CHAR_BIT || _I == 0))
			os << ' ';
	}
	return os;
}
#endif

_NSTD_END
#endif