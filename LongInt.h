#pragma once
#ifndef _NSTD_LONGINT_
#define _NSTD_LONGINT_

#include <xmemory>
#include <type_traits>
#include "Defines.h"
#include "Pair.h"

_NSTD_BEGIN

template <typename _Storage_type = uchar, typename _Alloc = _STD allocator<_Storage_type>>
class LongInt {
	using _Alty        = _STD _Rebind_alloc_t<_Alloc, _Storage_type>;
	using _Alty_traits = _STD allocator_traits<_Alty>;
	using _Mysize_t    = typename _Alty::size_type;
	using _Mystorage_t = typename _Alty_traits::value_type;
	using _Myptr_t     = typename _Alty_traits::pointer;

	static constexpr _Mysize_t _Mybytesize = sizeof(_Mystorage_t);
	static constexpr _Mysize_t _Mybitsize  = _Mybytesize * CHAR_BIT;

	// C2248
	template <typename, typename> friend class LongInt;

public:
	using _Mypair_t = _NSTD pair<_Mysize_t, _Myptr_t>;

	// Max 256 bytes (exclusive)
	static constexpr _Mysize_t _Maxsize = 256 / _Mybytesize;

	/*	Utility(?)
	*	static constexpr auto _MAX_OF = []() -> LongInt {
	*		return ~LongInt().grow(LongInt::_Maxsize - 1);
	*	};
	*/

	enum SHIFT_DIRECTION { LEFT = 0, RIGHT };
	template <typename st1, typename st2>
	_NODISCARD static st1 _Real_shift(st1 num, const st2& shift, const SHIFT_DIRECTION& dir) {
		_Real_shift_set(num, shift, dir);
		return num;
	}
	template <typename st1, typename st2>
	static void _Real_shift_set(st1& num, const st2& shift, const SHIFT_DIRECTION& dir) {
		if (dir == RIGHT) {
			_NSTD_FOR_I(shift / 64)
				(num >>= 63) >>= 1;
			num >>= shift % 64;
		} else {
			_NSTD_FOR_I(shift / 64)
				(num <<= 63) <<= 1;
			num <<= shift % 64;
		}
	}

	// Utility(?)
	template <typename st1, typename st2>
	_NODISCARD static constexpr auto const& _Min(const st1& first, const st2& second) {
		return first > second ? second : first;
	}

public:
	using allocator_type = _Alty;
	using storage_type   = _Mystorage_t;
	using size_type      = _Mysize_t;
	using pointer_type   = _Myptr_t;

public:
	LongInt()                                     : _Mypair(_Gen_basic()) {}
	LongInt(const LongInt& other)				  : _Mypair(_Gen_basic()) { _Set_to(other); }
	template <typename T> LongInt(const T& other) : _Mypair(_Gen_basic()) { _Set_to(other); }

/*	DEPRECATED - infinite production of LongInt objects
*	template <typename size_type>
*	LongInt(size_type&& count)									  : _Mypair(_Gen_basic()) { add(count); }
*/
	~LongInt() { _Tidy(); }

	// Grow by (in bytes)
	template <typename T>
	LongInt& grow(const T& size) {
		_Grow_by(size);
		return *this;
	}

	LongInt& shrink_fit() {
		if(_Myhighest() / _Mybitsize + 1 < _Mysize())
			_Grow_to(_Myhighest() / _Mybitsize + 1);
		return *this;
	}

	template <typename T>
	_NODISCARD operator T() const {
		T out(0);
		_NSTD_FOR_I_REVERSE(_Mysize()) {
			out += _Myarr()[_I];
			if(_I)
				out = _Real_shift(out, _Mybitsize, LEFT);
		}
		return out;
	}

	_NODISCARD explicit operator bool() const {
		_NSTD_FOR_I(_Mysize())
			if(_Myarr()[_I])
				return true;
		return false;
	}

	template <typename other_storage_t, typename other_alloc_t>
	explicit operator LongInt<other_storage_t, other_alloc_t>() const {
		return LongInt<other_storage_t, other_alloc_t>(*this);
	}

	_NODISCARD _Mysize_t size() {
		return _Mysize();
	}
	_NODISCARD const _Mysize_t size() const {
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

	LongInt& add(const LongInt& other) {
		if(&other == this || !other)
			return *this;
		_Grow_if(other._Mysize());
		_NSTD_FOR_I(_Min(_Mysize(), other._Mysize())) {
			if(_Overflows(other._Myarr()[_I], _Myarr()[_I]))
				_Unary_add(_I + 1);
			_Myarr()[_I] += other._Myarr()[_I];
		}
		return *this;
	}
	template <typename T>
	LongInt& operator+= (const T& count) {
		return add(LongInt(count));
	}
	template <>
	LongInt& operator+= (const LongInt& count) {
		return add(count);
	}
	template <typename T>
	_NODISCARD LongInt operator+ (const T& count) const {
		return LongInt(*this).operator+= (count);
	}

	LongInt& subtract(const LongInt& other) {
		if(&other == this || !other)
			return *this;
		_NSTD_ASSERT(*this >= other,
			"LongInt is unsigned -- subtracting number larger than LongInt object");
		_NSTD_FOR_I(_Min(other._Mysize(), _Mysize())) {
			if(_Myarr()[_I] < other._Myarr()[_I])
				_Unary_sub(_I + 1);
			_Myarr()[_I] -= other._Myarr()[_I];
		}
		return *this;
	}
	template <typename T>
	LongInt& operator-= (const T& count) {
		return subtract(LongInt(count));
	}
	template <>
	LongInt& operator-= (const LongInt& count) {
		return subtract(count);
	}
	template <typename T>
	_NODISCARD LongInt operator- (const T& count) const {
		return LongInt(*this).operator-= (count);
	}
	// LongInt is unsigned
	LongInt operator- () = delete;

	LongInt& multiply(const LongInt& other) {
		LongInt tcache(*this), ocache(other);
		_Set_zero();
		while(ocache) {
			if(ocache & LongInt(1))
				add(tcache);
			tcache <<= 1Ui64;
			ocache >>= 1Ui64;
		}
		return *this;
	}
	template <typename T>
	LongInt& operator*= (const T& count) {
		return multiply(LongInt(count));
	}
	LongInt& operator*= (const LongInt& count) {
		return multiply(count);
	}
	template <typename T>
	LongInt& operator* (const T& count) {
		return LongInt(*this).operator*= (count);
	}

	LongInt& divide(const LongInt& divisor) {
		LongInt dividend(*this);
		_Set_zero();
		_NSTD_FOR_I_REVERSE(dividend._Myhighest() - divisor._Myhighest() + 1) {
			if(dividend >= divisor << _I) {
				uint t(dividend);
				dividend -= divisor << _I;
				_Myarr()[_I / _Mybitsize] |= _GET_BIT(_Mystorage_t, _I % _Mybitsize);
			}
		}
		return *this;
	}
	template <typename T>
	LongInt& operator/= (const T& divisor) {
		return divide(LongInt(divisor));
	}
	template <>
	LongInt& operator/= (const LongInt& divisor) {
		return divide(divisor);
	}
	template <typename T>
	_NODISCARD LongInt operator/ (const T& divisor) const {
		return LongInt(*this).operator/= (divisor);
	}

	LongInt& mod(const LongInt& count) {
		if(_Highest(count) > _Myhighest())
			return *this;

		_NSTD_FOR_I_REVERSE(_Myhighest() + LongInt(1))
			if(this->operator> (count << _I))
				this->operator-= (count << _I);

		return *this;
	}
	template <typename T>
	LongInt& operator%= (const T& count) {
		return mod(LongInt(count));
	}
	template <>
	LongInt& operator%= (const LongInt& count) {
		return mod(count);
	}
	template <typename T>
	_NODISCARD LongInt operator% (const T& count) const {
		return LongInt(*this).operator%= (count);
	}

	LongInt& shift(_Mysize_t count, SHIFT_DIRECTION dir) {
		if(!count) 
			return *this;
		
		if(dir == LEFT)
			_Grow_if((_Myhighest() + count) / _Mybitsize + 1);
		LongInt cache(*this);
		cache._Grow_if(_Mysize());
		_Set_zero();
		_NSTD_FOR_I(_Mysize()) {
			_Mystorage_t lOrderP(count % _Mybitsize);
			if(dir == RIGHT) {
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
	template <typename T>
	LongInt& operator<<= (const T& count) {
		return shift(static_cast<_Mysize_t>(count), LEFT);
	}
	template <typename T>
	_NODISCARD LongInt operator<< (const T& count) const {
		return LongInt(*this).shift(count, LEFT);
	}
	template <typename T>
	LongInt& operator>>= (const T& count) {
		return shift(static_cast<_Mysize_t>(count), RIGHT);
	}
	template <typename T>
	_NODISCARD LongInt operator>> (const T& count) const {
		return LongInt(*this).shift(count, RIGHT);
	}

	// Logical operators vvv
	template <typename T>
	LongInt& operator= (const T& other) {
		if(this != _STD addressof(other))
			_Set_to(other);
		return *this;
	}

	_NODISCARD LongInt operator~ () const {
		LongInt cache(*this);
		cache._Grow_if(_Mysize());
		_NSTD_FOR_I(cache._Mysize())
			cache._Myarr()[_I] = ~_Myarr()[_I];
		return cache;
	}

	LongInt& operator-- () {
		return subtract(1);
	}
	LongInt& operator++ () {
		return add(1);
	}

	template <typename T>
	LongInt& operator^= (const T& count) {
		return this->operator^= (LongInt(count));
	}
	template <>
	LongInt& operator^= (const LongInt& other) {
		_NSTD_FOR_I(_Min(other._Mysize(), _Mysize()))
			_Myarr()[_I] ^= other._Myarr()[_I];
		return *this;
	}
	template <typename T>
	_NODISCARD LongInt operator^ (const T& count) const {
		return LongInt(*this).operator^= (count);
	}


	template <typename T>
	LongInt& operator&= (const T& count) {
		return this->operator&= (LongInt(count));
	}
	template <>
	LongInt& operator&= (const LongInt& other) {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] &= other._Myarr()[_I];
		return *this;
	}
	template <typename T>
	_NODISCARD LongInt operator& (const T& count) const {
		return LongInt(*this).operator&= (count);
	}

	template <typename T>
	LongInt& operator|= (const T& count) {
		return this->operator|= (LongInt(count));
	}
	template <>
	LongInt& operator|= (const LongInt& other) {
		_NSTD_FOR_I(_Min(other._Mysize(), _Mysize()))
			_Myarr()[_I] |= other._Myarr()[_I];
		return *this;
	}
	template <typename T>
	_NODISCARD LongInt operator| (const T& count) const {
		return LongInt(*this).operator|= (count);
	}

	// Comparisons
	template <typename T>
	_NODISCARD bool operator!= (const T& count) const {
		return this->operator!= (LongInt(count));
	}
	template <>
	_NODISCARD bool operator!= (const LongInt& other) const {
		if(_Myhighest() != other._Myhighest())
			return true;
		_NSTD_FOR_I(_Min(_Mysize(), other._Mysize()))
			if(_Myarr()[_I] != other._Myarr()[_I])
				return true;
		return false;
	}
	template <typename T>
	_NODISCARD bool operator== (const T& count) const {
		return !this->operator!= (count);
	}

	template <typename T>
	_NODISCARD bool operator> (const T& count) const {
		return this->operator> (LongInt(count));
	}
	template <>
	_NODISCARD bool operator> (const LongInt& other) const {
		if(_Myhighest() != other._Myhighest())
			return _Myhighest() > other._Myhighest();
		_NSTD_FOR_I_REVERSE(_Min(_Mysize(), other._Mysize()))
			if(_Myarr()[_I] != other._Myarr()[_I])
				return _Myarr()[_I] > other._Myarr()[_I];
		return false;
	}
	template <typename T>
	_NODISCARD bool operator>= (const T& count) const {
		return this->operator> (count) || this->operator== (count);
	}
	template <typename T>
	_NODISCARD bool operator< (const T& count) const {
		return !this->operator>= (count);
	}
	template <typename T>
	_NODISCARD bool operator<= (const T& count) const {
		return !this->operator> (count);
	}

#ifdef _COMPARE_
	template <typename T>
	_NODISCARD _STD strong_ordering operator<=> (const T& count) const {
		if(this->operator> (count))
			return _STD strong_ordering::greater;
		else if(this->operator== (count))
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
	_NODISCARD static _Mypair_t _Gen_basic() {
		_Alty alloc;
		_Myptr_t p(alloc.allocate(1));
		_Alty_traits::construct(alloc, p, 0);
		return _Mypair_t(1, p);
	}

	// Overflow operators
	void _Unary_add(const _Mysize_t& _Index) {
		_Grow_if(_Index + 1);
		if(!++_Myarr()[_Index])
			_Unary_add(_Index + 1);
	}
	void _Unary_sub(const _Mysize_t& _Index) {
		_NSTD_ASSERT(_Mysize() > _Index,
			"_Unary_sub::_Index out of bounds");
		if(!_Myarr()[_Index])
			_Unary_sub(_Index + 1);
		_Myarr()[_Index]--;
	}

	template <typename T>
	void _Grow_if(const T& new_size, const bool b = true) {
		if(b && _Mysize() < new_size)
			_Grow_to(new_size);
	}
	template <typename T>
	void _Grow_by(const T& size) {
		_Grow_to(_Mysize() + size);
	}
	template <typename T>
	void _Grow_to(const T& new_size) {
		_NSTD_ASSERT(new_size > 0, "Cannot set LongInt size to or below 0");
		_NSTD_ASSERT(new_size <= _Maxsize, "LongInt grown to size above _Maxsize");

		_Alty alloc;
		_Mypair_t cache(_Mypair);
		_Myarr() = alloc.allocate(new_size);
		_NSTD_FOR_I(new_size)
			_Alty_traits::construct(alloc, (_Myarr() + _I), 0);
		_NSTD_FOR_I(_Min(cache.first, new_size))
			_Myarr()[_I] = cache.second[_I];
		_Alty_traits::deallocate(alloc, cache.second, cache.first);
		cache.second = nullptr;
		_Mysize() = new_size;
	}
	
	void _Set_zero() {
		_NSTD_FOR_I(_Mysize())
			_Myarr()[_I] = _Mystorage_t(0);
	}

	template <typename size_type1, typename size_type2>
	_NODISCARD static const bool _Overflows(size_type1 first, size_type2 second) {
		if(_HIGH_BIT(_Mystorage_t) & (first & second)) 
			return true;
		if(_HIGH_BIT(_Mystorage_t) & (first | second)) 
			return _Overflows(first <<= 1, second <<= 1);
		return false;
	}

	template <typename T> typename _STD enable_if_t<!_STD is_unsigned_v<T>> static _Make_abs(T& n) { n = n < 0 ? -n : n; }
	template <typename T> typename _STD enable_if_t< _STD is_unsigned_v<T>> static _Make_abs(T& n) {}

	template <typename T>
	static const _Mysize_t _Highest(T count) {
		_Make_abs(count);
		if(!count)
			return 0;
		const T cache(count);
		_NSTD_FOR_I(cache) {
			if(_Real_shift(count, _Mybitsize, RIGHT)) {
				_Real_shift_set(count, _Mybitsize, RIGHT);
				continue;
			}
			_NSTD_FOR_J_REVERSE(_Mybitsize)
				if(_GET_BIT(_Mystorage_t, _J) & count)
					return _I * _Mybitsize + _J;
		}
		return 0;
	}
	template <typename other_storage_t, typename other_alloc_t>
	static const _Mysize_t _Highest(const LongInt<other_storage_t, other_alloc_t>& other) {
		_NSTD_FOR_I_REVERSE(other._Mysize())
			if(other._Myarr()[_I])
				_NSTD_FOR_J_REVERSE(other._Mybitsize)
					if(other._Myarr()[_I] & _GET_BIT(other_storage_t, _J))
						return _I * other._Mybitsize + _J;
		return 0;
	}

	const _Mysize_t _Myhighest() const {
		return _Highest(*this);
	}

	template <typename T>
	void _Set_to(const T& num) {
		_NSTD_ASSERT(num >= 0,
			"Setting LongInt object to negative value");
		_Set_zero();
		_Grow_if(_Highest(num) / _Mybitsize + 1);
		_NSTD_FOR_I(_Highest(num) + 1) 
			_Myarr()[_I / _Mybitsize] |= ((num & _GET_BIT(T, _I)) ? T(1) << _I % _Mybitsize : 0);
	}
	template <typename other_storage_t, typename other_alloc_t>
	void _Set_to(const LongInt<other_storage_t, other_alloc_t>& other) {
		_Set_zero();
		_Grow_if(other._Mysize() * other._Mybitsize / _Mybitsize + 1);
		_NSTD_FOR_I(other._Myhighest())
			_Myarr()[_I / _Mybitsize] |= other._Myarr()[_I / other._Mybitsize] & _GET_BIT(other_storage_t, _I % other._Mybitsize);
	}
	template <>
	void _Set_to(const LongInt& other) {
		_Set_zero();
		_Grow_if(other._Mysize());
		_NSTD_FOR_I(other._Mysize())
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
		if(!(_I % CHAR_BIT || _I == 0))
			os << ' ';
	}
	return os;
}
#endif

#ifdef _NSTD_LONGINT_TESTER_
#pragma push_macro("_CATCHBLOCK")
#define _CATCHBLOCK(msg) catch(...) { b = true; } _STD cout << "LongInt " << (b ? "FAIL" : "SUCCESS") << ": " << msg << '\n'; b = false;

struct _LONGINT_TESTER {
	static void test1() { // Constructors
		bool b = false;
		try {
			LongInt<> li;
		} _CATCHBLOCK("nullary constructor");

		try {
			LongInt<> li(0);
		} _CATCHBLOCK("zero constructor");

		try {
			LongInt<> li(982);
		} _CATCHBLOCK("nonzero constructor");

		try {
			LongInt<> li1;
			LongInt<> li2(li1);
		} _CATCHBLOCK("nullary other constructor");

		try {
			LongInt<> li1(0);
			LongInt<> li2(li1);
		} _CATCHBLOCK("zero other constructor");

		try {
			LongInt<> li1(982);
			LongInt<> li2(li1);
		} _CATCHBLOCK("nonzero other constructor");

		try {
			LongInt<> li1(982);
			LongInt<int> li2(li1);
		} _CATCHBLOCK("different storage other constructor");
	}

	static void test2() { // Comparitors
		bool b = false;
		try {
			LongInt<> li(2435298);
			if(!(li == 2435298))
				throw _STD exception();
		} _CATCHBLOCK("==");

		try {
			LongInt<> li(2435298);
			if(!(li != 2435298 - 1))
				throw _STD exception();
		} _CATCHBLOCK("!=");

		try {
			LongInt<> li(2435298);
			if(!(li > 2435298 - 1))
				throw _STD exception();
		} _CATCHBLOCK(">");

		try {
			LongInt<> li(2435298);
			if(!(li >= 2435298 - 1) || !(li >= 2435298))
				throw _STD exception();
		} _CATCHBLOCK(">=");

		try {
			LongInt<> li(2435298);
			if(!(li < 2435298 + 1))
				throw _STD exception();
		} _CATCHBLOCK("<");

		try {
			LongInt<> li(2435298);
			if(!(li <= 2435298 + 1) || !(li <= 2435298))
				throw _STD exception();
		} _CATCHBLOCK("<=");		
	}

	static void test3() { // Operators
		bool b = false;
		try {
			LongInt<> li(2435298);
			li += 2341298;
			if(li != 2435298 + 2341298)
				throw _STD exception();
		} _CATCHBLOCK("+=");

		try {
			LongInt<> li(2435298);
			if(li + 2341298 != 2435298 + 2341298)
				throw _STD exception();
		} _CATCHBLOCK("+");

		try {
			LongInt<> li(2435298);
			li -= 2341298;
			if(li != 2435298 - 2341298)
				throw _STD exception();
		} _CATCHBLOCK("-=");

		try {
			LongInt<> li(2435298);
			if(li - 2341298 != 2435298 - 2341298)
				throw _STD exception();
		} _CATCHBLOCK("-");

		try {
			LongInt<> li(2435298);
			li /= 34;
			if(li != 2435298Ui64 / 34)
				throw _STD exception();
		} _CATCHBLOCK("/=");

		try {
			LongInt<> li(2435298);
			if(li / 34 != 2435298Ui64 / 34)
				throw _STD exception();
		} _CATCHBLOCK("/");

		try {
			LongInt<> li(2435298);
			li %= 34;
			if(li != 2435298 % 34)
				throw _STD exception();
		} _CATCHBLOCK("%=");

		try {
			LongInt<> li(2435298);
			if(li % 34 != 2435298 % 34)
				throw _STD exception();
		} _CATCHBLOCK("%");


	}
};

#pragma pop_macro("_CATCHBLOCK")
#endif// _NSTD_LONGINT_TESTER_

_NSTD_END
#endif