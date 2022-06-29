#pragma once
#ifndef _NSTD_DEQUE_
#define _NSTD_DEQUE_

#include <deque>
#include "Defines.h"

_NSTD_BEGIN

template <typename _Ty, typename _Alloc = _STD allocator<_Ty>>
class deque : _STD deque<_Ty, _Alloc> {};

class _Bproxy {
	_NSTD_UNSIGNED

	uint relative_pos_;
	uchar* char_;
public:
	_Bproxy(uchar* c, uint rp) : relative_pos_(rp), char_(c) {}
	_Bproxy(uchar& c, uint rp) : relative_pos_(rp), char_(&c) {}
	_Bproxy(_Bproxy& other) : relative_pos_(other.relative_pos_), char_(other.char_) {}
	// funcs
	_Bproxy& set(bool b) {
		b ? *char_ |=  _BITX(relative_pos_)
		  : *char_ &= ~_BITX(relative_pos_);
		return *this;
	}
	// ops
	operator bool() const {
		return *char_ & _BITX(relative_pos_);
	}
	bool operator* () const {
		return static_cast<bool>(*this);
	}
	// One cannot access internal bools via classic pointers
	// This class should in itself be used as a pointer
	bool* operator& () = delete;
	_Bproxy& operator= (bool b) {
		return set(b);
	}
	_Bproxy& operator= (const _Bproxy& other) {
		relative_pos_ = other.relative_pos_;
		char_ = other.char_;
	}
};

template <typename _Mydeque>
class _Db_unchecked_const_iterator {
private:
	using _Size_type = typename _Mydeque::size_type;

	static constexpr int _Block_size = _Mydeque::_Block_size;

public:
	using iterator_category = _STD random_access_iterator_tag;

	using value_type      = typename _Mydeque::value_type;
	using difference_type = typename _Mydeque::difference_type;
	using pointer         = typename _Mydeque::const_pointer;
	using reference       = typename _Mydeque::const_reference;

	_Db_unchecked_const_iterator() noexcept : _Mycont(), _Myoff(0) {}

	_Db_unchecked_const_iterator(_Size_type _Off, const _STD _Container_base12* _Pdeque) noexcept
		: _Mycont(static_cast<const _Mydeque*>(_Pdeque)), _Myoff(_Off) {}
	
	_NODISCARD reference operator*() const noexcept {
		_Size_type _Block = _Mycont->_Getblock(_Myoff);
		_Size_type _Off   = _Myoff % _Block_size;
		return reference(_Mycont->_Map[_Block][_Off / CHAR_BIT], _Off % CHAR_BIT);
	}
	// Booleans have no op-> functionality
	// Note: same is true for _Bool_proxy
	pointer operator->() const = delete;
	
	_Db_unchecked_const_iterator& operator++() noexcept {
		++_Myoff;
		return *this;
	}

	_Db_unchecked_const_iterator operator++(int) noexcept {
		_Db_unchecked_const_iterator _Tmp = *this;
		++_Myoff;
		return _Tmp;
	}

	_Db_unchecked_const_iterator& operator--() noexcept {
		--_Myoff;
		return *this;
	}

	_Db_unchecked_const_iterator operator--(int) noexcept {
		_Db_unchecked_const_iterator _Tmp = *this;
		--_Myoff;
		return _Tmp;
	}

	_Db_unchecked_const_iterator& operator+=(const difference_type _Off) noexcept {
		_Myoff += _Off;
		return *this;
	}

	_NODISCARD _Db_unchecked_const_iterator operator+(const difference_type _Off) const noexcept {
		_Db_unchecked_const_iterator _Tmp = *this;
		_Tmp += _Off;
		return _Tmp;
	}

	_Db_unchecked_const_iterator& operator-=(const difference_type _Off) noexcept {
		_Myoff -= _Off;
		return *this;
	}

	_NODISCARD _Db_unchecked_const_iterator operator-(const difference_type _Off) const noexcept {
		_Db_unchecked_const_iterator _Tmp = *this;
		_Tmp -= _Off;
		return _Tmp;
	}

	_NODISCARD difference_type operator-(const _Db_unchecked_const_iterator& _Right) const noexcept {
		return static_cast<difference_type>(_Myoff - _Right._Myoff);
	}

	_NODISCARD reference operator[](const difference_type _Off) const noexcept {
		return *(*this + _Off);
	}

	_NODISCARD bool operator==(const _Db_unchecked_const_iterator& _Right) const noexcept {
		return _Myoff == _Right._Myoff;
	}

	_NODISCARD _STD strong_ordering operator<=>(const _Db_unchecked_const_iterator& _Right) const noexcept {
		return _Myoff <=> _Right._Myoff;
	}
	
	const _STD _Container_base12* _Getcont() const noexcept { // get container pointer
		return _Mycont;
	}

	const _Mydeque* _Mycont;
	_Size_type _Myoff; // offset of element in deque
};

template <class _Mydeque>
class _Db_unchecked_iterator : public _Db_unchecked_const_iterator<_Mydeque> {
private:
	using _Size_type = typename _Mydeque::size_type;
	using _Mybase = _Db_unchecked_const_iterator<_Mydeque>;

public:
	using iterator_category = _STD random_access_iterator_tag;

	using value_type      = typename _Mydeque::value_type;
	using difference_type = typename _Mydeque::difference_type;
	using pointer         = typename _Mydeque::pointer;
	using reference       = typename _Mydeque::reference;

	using _Mybase::_Mybase;

	_NODISCARD reference operator*() const noexcept {
		return const_cast<reference>(_Mybase::operator*());
	}

	pointer operator->() const = delete;

	_Db_unchecked_iterator& operator++() noexcept {
		_Mybase::operator++();
		return *this;
	}

	_Db_unchecked_iterator operator++(int) noexcept {
		_Db_unchecked_iterator _Tmp = *this;
		_Mybase::operator++();
		return _Tmp;
	}

	_Db_unchecked_iterator& operator--() noexcept {
		_Mybase::operator--();
		return *this;
	}

	_Db_unchecked_iterator operator--(int) noexcept {
		_Db_unchecked_iterator _Tmp = *this;
		_Mybase::operator--();
		return _Tmp;
	}

	_Db_unchecked_iterator& operator+=(const difference_type _Off) noexcept {
		_Mybase::operator+=(_Off);
		return *this;
	}

	_NODISCARD _Db_unchecked_iterator operator+(const difference_type _Off) const noexcept {
		_Db_unchecked_iterator _Tmp = *this;
		_Tmp += _Off;
		return _Tmp;
	}

	_Db_unchecked_iterator& operator-=(const difference_type _Off) noexcept {
		_Mybase::operator-=(_Off);
		return *this;
	}

	_NODISCARD _Db_unchecked_iterator operator-(const difference_type _Off) const noexcept {
		_Db_unchecked_iterator _Tmp = *this;
		_Tmp -= _Off;
		return _Tmp;
	}

	_NODISCARD difference_type operator-(const _Mybase& _Right) const noexcept {
		return _Mybase::operator-(_Right);
	}

	_NODISCARD reference operator[](const difference_type _Off) const noexcept {
		return const_cast<reference>(_Mybase::operator[](_Off));
	}
};

template <class _Mydeque>
class _Db_const_iterator : public _STD _Iterator_base12 {
private:
	using _Size_type = typename _Mydeque::size_type;

	static constexpr int _Block_size = _Mydeque::_Block_size;

public:
	using iterator_category = _STD random_access_iterator_tag;

	using value_type      = typename _Mydeque::value_type;
	using difference_type = typename _Mydeque::difference_type;
	using pointer         = typename _Mydeque::const_pointer;
	using reference       = typename _Mydeque::const_reference;

	using _Mydeque_t = _Mydeque; // helper for expression evaluator
	enum { _EEN_DS = _Block_size }; // helper for expression evaluator
	_Db_const_iterator() noexcept : _Myoff(0) {
		_Setcont(nullptr);
	}

	_Db_const_iterator(_Size_type _Off, const _STD _Container_base12* _Pdeque) noexcept : _Myoff(_Off) {
		_Setcont(static_cast<const _Mydeque*>(_Pdeque));
	}

	_NODISCARD reference operator*() const noexcept {
		const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
#if _ITERATOR_DEBUG_LEVEL != 0
		_STL_VERIFY(_Mycont, "cannot dereference value-initialized deque iterator");
		_STL_VERIFY(_Mycont->_Myoff <= this->_Myoff && this->_Myoff < _Mycont->_Myoff + _Mycont->_Mysize,
			"cannot deference out of range deque iterator");
#endif // _ITERATOR_DEBUG_LEVEL != 0

		_Size_type _Block = _Mycont->_Getblock(_Myoff);
		_Size_type _Off   = _Myoff % _Block_size;
		return reference(_Mycont->_Map[_Block][_Off / CHAR_BIT], _Off % CHAR_BIR);
	}

	pointer operator->() const = delete;

	_Db_const_iterator& operator++() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
		_STL_VERIFY(_Mycont, "cannot increment value-initialized deque iterator");
		_STL_VERIFY(this->_Myoff < _Mycont->_Myoff + _Mycont->_Mysize, "cannot increment deque iterator past end");
#endif // _ITERATOR_DEBUG_LEVEL != 0

		++_Myoff;
		return *this;
	}

	_Db_const_iterator operator++(int) noexcept {
		_Db_const_iterator _Tmp = *this;
		++* this;
		return _Tmp;
	}

	_Db_const_iterator& operator--() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
		const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
		_STL_VERIFY(_Mycont, "cannot decrement value-initialized deque iterator");
		_STL_VERIFY(_Mycont->_Myoff < this->_Myoff, "cannot decrement deque iterator before begin");
#endif // _ITERATOR_DEBUG_LEVEL != 0

		--_Myoff;
		return *this;
	}

	_Db_const_iterator operator--(int) noexcept {
		_Db_const_iterator _Tmp = *this;
		--* this;
		return _Tmp;
	}

	_Db_const_iterator& operator+=(const difference_type _Off) noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
		if (_Off != 0) {
			const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
			_STL_VERIFY(_Mycont, "cannot seek value-initialized deque iterator");
			_STL_VERIFY(
				_Mycont->_Myoff <= this->_Myoff + _Off && this->_Myoff + _Off <= _Mycont->_Myoff + _Mycont->_Mysize,
				"cannot seek deque iterator out of range");
		}
#endif // _ITERATOR_DEBUG_LEVEL != 0

		_Myoff += _Off;
		return *this;
	}

	_NODISCARD _Db_const_iterator operator+(const difference_type _Off) const noexcept {
		_Db_const_iterator _Tmp = *this;
		_Tmp += _Off;
		return _Tmp;
	}

	_Db_const_iterator& operator-=(const difference_type _Off) noexcept {
		return *this += -_Off;
	}

	_NODISCARD _Db_const_iterator operator-(const difference_type _Off) const noexcept {
		_Db_const_iterator _Tmp = *this;
		_Tmp -= _Off;
		return _Tmp;
	}

	_NODISCARD difference_type operator-(const _Db_const_iterator& _Right) const noexcept {
		_Compat(_Right);
		return static_cast<difference_type>(this->_Myoff - _Right._Myoff);
	}

	_NODISCARD reference operator[](const difference_type _Off) const noexcept {
		return *(*this + _Off);
	}

	_NODISCARD bool operator==(const _Db_const_iterator& _Right) const noexcept {
		_Compat(_Right);
		return this->_Myoff == _Right._Myoff;
	}

	_NODISCARD _STD strong_ordering operator<=>(const _Db_const_iterator& _Right) const noexcept {
		_Compat(_Right);
		return this->_Myoff <=> _Right._Myoff;
	}

	void _Compat(const _Db_const_iterator& _Right) const noexcept { // test for compatible iterator pair
#if _ITERATOR_DEBUG_LEVEL == 0
		(void)_Right;
#else // _ITERATOR_DEBUG_LEVEL == 0
		_STL_VERIFY(this->_Getcont() == _Right._Getcont(), "deque iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL == 0
	}

	void _Setcont(const _Mydeque* _Pdeque) noexcept { // set container pointer
		this->_Adopt(_Pdeque);
	}

	using _Prevent_inheriting_unwrap = _Db_const_iterator;

	_NODISCARD _Db_unchecked_const_iterator<_Mydeque> _Unwrapped() const noexcept {
		return { this->_Myoff, this->_Getcont() };
	}

	void _Verify_offset(const difference_type _Off) const noexcept {
#if _ITERATOR_DEBUG_LEVEL == 0
		(void)_Off;
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv
		if (_Off != 0) {
			const auto _Mycont = static_cast<const _Mydeque*>(this->_Getcont());
			_STL_VERIFY(_Mycont, "cannot use value-initialized deque iterator");
			_STL_VERIFY(
				_Mycont->_Myoff <= this->_Myoff + _Off && this->_Myoff + _Off <= _Mycont->_Myoff + _Mycont->_Mysize,
				"cannot seek deque iterator out of range");
		}
#endif // _ITERATOR_DEBUG_LEVEL == 0
	}

#if _ITERATOR_DEBUG_LEVEL != 0
	friend void _Verify_range(const _Db_const_iterator& _First, const _Db_const_iterator& _Last) noexcept {
		// note _Compat check inside operator<=
		_STL_VERIFY(_First <= _Last, "deque iterators transposed");
	}
#endif // _ITERATOR_DEBUG_LEVEL != 0

	void _Seek_to(const _Db_unchecked_const_iterator<_Mydeque>& _UIt) noexcept {
		_Myoff = _UIt._Myoff;
	}

	_Size_type _Myoff; // offset of element in deque
};

template <class _Mydeque>
class _Db_iterator : public _Db_const_iterator<_Mydeque> {
private:
	using _Size_type = typename _Mydeque::size_type;
	using _Mybase    = _Db_const_iterator<_Mydeque>;

public:
	using _Deque_unchecked_type = _Db_unchecked_iterator<_Mydeque>;
	using iterator_category     = _STD random_access_iterator_tag;

	using value_type      = typename _Mydeque::value_type;
	using difference_type = typename _Mydeque::difference_type;
	using pointer         = typename _Mydeque::pointer;
	using reference       = typename _Mydeque::reference;

	using _Mybase::_Mybase;

	_NODISCARD reference operator*() const noexcept {
		return const_cast<reference>(_Mybase::operator*());
	}

	pointer operator->() = delete;

	_Db_iterator& operator++() noexcept {
		_Mybase::operator++();
		return *this;
	}

	_Db_iterator operator++(int) noexcept {
		_Db_iterator _Tmp = *this;
		_Mybase::operator++();
		return _Tmp;
	}

	_Db_iterator& operator--() noexcept {
		_Mybase::operator--();
		return *this;
	}

	_Db_iterator operator--(int) noexcept {
		_Db_iterator _Tmp = *this;
		_Mybase::operator--();
		return _Tmp;
	}

	_Db_iterator& operator+=(const difference_type _Off) noexcept {
		_Mybase::operator+=(_Off);
		return *this;
	}

	_NODISCARD _Db_iterator operator+(const difference_type _Off) const noexcept {
		_Db_iterator _Tmp = *this;
		_Tmp += _Off;
		return _Tmp;
	}

	_Db_iterator& operator-=(const difference_type _Off) noexcept {
		_Mybase::operator-=(_Off);
		return *this;
	}

	using _Mybase::operator-;

	_NODISCARD _Db_iterator operator-(const difference_type _Off) const noexcept {
		_Db_iterator _Tmp = *this;
		_Tmp -= _Off;
		return _Tmp;
	}

	_NODISCARD reference operator[](const difference_type _Off) const noexcept {
		return const_cast<reference>(_Mybase::operator[](_Off));
	}

	using _Prevent_inheriting_unwrap = _Db_iterator;

	_NODISCARD _Db_unchecked_iterator<_Mydeque> _Unwrapped() const noexcept {
		return { this->_Myoff, this->_Getcont() };
	}
};

template <class _Value_type, class _Size_type, class _Difference_type, class _Pointer, class _Const_pointer,
	class _Reference, class _Const_reference, class _Mapptr_type>
struct _Db_iter_types {
	using value_type      = _Value_type;
	using size_type       = _Size_type;
	using difference_type = _Difference_type;
	using pointer         = _Pointer;
	using const_pointer   = _Const_pointer;
	using reference       = _Reference;
	using const_reference = _Const_reference;
	using _Mapptr         = _Mapptr_type;
};

template <class _Val_types>
class _Db_val : public _STD _Container_base12 {
public:
	using value_type      = typename _Val_types::value_type;
	using size_type       = typename _Val_types::size_type;
	using difference_type = typename _Val_types::difference_type;
	using pointer         = typename _Val_types::pointer;
	using const_pointer   = typename _Val_types::const_pointer;
	using reference       = typename _Val_types::reference;
	using const_reference = typename _Val_types::const_reference;
	using _Mapptr         = typename _Val_types::_Mapptr;

private:
	static constexpr size_t _Bytes = sizeof(value_type);

public:
	static constexpr int _Block_size = 16;

	_Db_val() noexcept : _Map(), _Mapsize(0), _Myoff(0), _Mysize(0) {}

	size_type _Getblock(size_type _Off) const noexcept {
		// NB: _Mapsize and _Block_size are guaranteed to be powers of 2
		return (_Off / _Block_size) & (_Mapsize - 1);
	}

	_Mapptr _Map; // pointer to array of pointers to blocks
	size_type _Mapsize; // size of map array, zero or 2^N
	size_type _Myoff; // offset of initial element
	size_type _Mysize; // current length of sequence
};


// This is a class optimized specifically for use in huffman encoding
// See: Huff.h & Huff.cpp
template <typename _Alloc>
class deque<bool, _Alloc> {
	_NSTD_UNSIGNED
	
private:
	uchar* carr_;
	uint carr_sz_, Hpos_, Tpos_;

// ripped
	using _Alty           = _STD _Rebind_alloc_t<_Alloc, uchar>;
	using _Alty_traits    = _STD allocator_traits<_Alty>;
	using _Alpty          = _STD _Rebind_alloc_t<_Alloc, typename _Alty_traits::pointer>;
	using _Alpty_traits   = _STD allocator_traits<_Alpty>;
	using _Mapptr         = typename _Alpty_traits::pointer;
	using _Alproxy_ty     = _STD _Rebind_alloc_t<_Alty, _STD _Container_proxy>;
	using _Alproxy_traits = _STD allocator_traits<_Alproxy_ty>;

	using _Scary_val = _Db_val<
		_Db_iter_types<
			bool, typename _Alty_traits::size_type, typename _Alty_traits::difference_type, 
				typename _Alty_traits::pointer, typename _Alty_traits::const_pointer, _Bproxy, const _Bproxy, _Mapptr>>;

	static constexpr int _Block_size = _Scary_val::_Block_size;

public:
	using allocator_type  = _Alloc;
	using value_type      = bool;
	using size_type       = typename _Alty_traits::size_type;
	using difference_type = typename _Alty_traits::difference_type;
	using pointer         = typename _Alty_traits::pointer;
	using const_pointer   = typename _Alty_traits::const_pointer;
	using reference       = _Bproxy;
	using const_reference = const _Bproxy;

	using iterator                  = _Db_iterator                 <_Scary_val>;
	using const_iterator            = _Db_const_iterator           <_Scary_val>;
	using _Unchecked_iterator       = _Db_unchecked_iterator       <_Scary_val>;
	using _Unchecked_const_iterator = _Db_unchecked_const_iterator <_Scary_val>;

	using reverse_iterator       = _STD reverse_iterator<iterator>;
	using const_reverse_iterator = _STD reverse_iterator<const_iterator>;


public:
	deque(uint size = 0xff)   : carr_(new uchar[size / CHAR_BIT + 1]), carr_sz_(size / CHAR_BIT + 1), Hpos_(carr_sz_ / 2), Tpos_(Hpos_ - 1) {}
	deque(const deque& other) : carr_(new uchar[other.carr_sz_]),      carr_sz_(other.carr_sz_),      Hpos_(other.Hpos_),  Tpos_(other.Tpos_) {
		_NSTD_FOR_I(carr_sz_)
			carr_[_I] = other.carr_[_I];
	}


	// This is a utility.
	// Note: the caller must call delete[] on generated array themselves
	_NODISCARD static uchar* gen_bitX() {
		uchar* nbitX(new uchar[CHAR_BIT]);
		_NSTD_FOR_I(CHAR_BIT)
			nbitX[_I] = _BITX(_I);
		return nbitX;
	}
	
/*  // Debugging
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
*/


	_NODISCARD uint size() {
		// inclusive
		return Tpos_ - Hpos_ + 1;
	}
	_NODISCARD uint real_size() {
		return carr_sz_;
	}
	_NODISCARD uint potential_size() {
		return carr_sz_ * CHAR_BIT - 1;
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
	// Note: expands backwards
	void resize(uint num_bools, bool init = false) {
		uint real_sz = static_cast<uint> (num_bools / CHAR_BIT) + 1;
		uchar* cashe(carr_);
		carr_ = init ? new uchar[real_sz](0xff) : new uchar[real_sz](0x00);
		if (real_sz >= carr_sz_) {
			_NSTD_FOR_I(carr_sz_)
				carr_[_I] = cashe[_I];
		} else /* truncation */ {
			_NSTD_FOR_I(real_sz)
				carr_[_I] = cashe[_I];
			Tpos_ = num_bools + 1;
		}
		delete[] cashe;
	}
	void resize_NOCOPY(uint num_bools, bool init = false) {
		delete[] carr_;
		carr_ = init ? new uchar[num_bools](0xff) : new uchar[num_bools](0x00);
		clear();
	}
	_NODISCARD reference front() {
		_NSTD_ASSERT(!empty(), "called front() on empty deque");
		return _Bproxy(carr_[static_cast<uint> (Hpos_ / CHAR_BIT)], Hpos_ % CHAR_BIT);
	}
	_NODISCARD const_reference front() const {
		return front();
	}
	reference pop_front() {
		_Bproxy out(front());
		++Hpos_;
		return out;
	}
	_NODISCARD reference back() {
		_NSTD_ASSERT(!empty(), "called back() on empty deque");
		return _Bproxy(carr_[static_cast<uint> (Tpos_ / CHAR_BIT)], Tpos_ % CHAR_BIT);
	}
	_NODISCARD const_reference back() const {
		return back();
	}
	reference pop_back() {
		_Bproxy out(back());
		--Tpos_;
		return out;
	}
	// Sum of shrink_front & shrink_back
	deque& shrink_fit() {
		return shrink_front().shrink_back();
	}
	deque& shrink_front() {
		if (static_cast<uint> (Hpos_ / CHAR_BIT)) {
			uint high_pos(static_cast<uint> (Hpos_ / CHAR_BIT));
			carr_sz_ -= high_pos;
			uchar* narr(new uchar[carr_sz_]);
			_NSTD_FOR_I(carr_sz_)
				narr[_I] = carr_[high_pos + _I];
			delete[] carr_;
			carr_ = narr;
		}
		return *this;
	}
	deque& shrink_back() {
		if (static_cast<uint> (Tpos_ / CHAR_BIT + 1) < carr_sz_) {
			carr_sz_ = static_cast<uint> (Tpos_ / CHAR_BIT + 1);
			uchar* narr(new uchar[carr_sz_]);
			_NSTD_FOR_I(carr_sz_)
				narr[_I] = carr_[_I];
			delete[] carr_;
			carr_ = narr;
		}
		return *this;
	}
	deque& push_back(bool b) {
		if (static_cast<uint> (++Tpos_ / CHAR_BIT) >= carr_sz_) {
			uchar* cashe(carr_);
			carr_ = new uchar[carr_sz_ *= 2](0);
			_NSTD_FOR_I(carr_sz_ / 2)
				carr_[_I] = cashe[_I];
			delete[] cashe;
		}
		set(Tpos_ - Hpos_, b);
		return *this;
	}
	// TODO: debug -- set(0,...) fails after several push fronts
	deque& push_front(bool b) {
		if (!--Hpos_) {
			uchar* cashe(carr_);
			carr_ = new uchar[carr_sz_ * 2](0);
			_NSTD_FOR_I(carr_sz_)
				carr_[_I + carr_sz_] = cashe[_I];
			delete[] cashe;
			Hpos_ += carr_sz_;
			Tpos_ += carr_sz_;
			carr_sz_ *= 2;
		}
		set(0, b);
		return *this;
	}

	// at(pos)
	_Bproxy operator[] (uint pos) {
		return at(pos);
	}
	_Bproxy at(uint pos) {
		uint real_pos(pos + Hpos_);
		_NSTD_ASSERT(real_pos <= Tpos_, "tried to access deque element outside deque bounds");
		return _Bproxy(carr_[static_cast<uint> (real_pos / CHAR_BIT)], real_pos % CHAR_BIT);
	}
	// set(pos, !at(pos))
	deque& flip(uint pos) {
		return set(pos, !at(pos));
	}
	deque& set(uint pos, bool b = true) {
		uint real_pos(pos + Hpos_);
		_NSTD_ASSERT(real_pos <= Tpos_, "tried to access deque element outside deque bounds");
		b ? carr_[static_cast<uint> (real_pos / CHAR_BIT)] |=  _BITX(real_pos % CHAR_BIT) 
		  : carr_[static_cast<uint> (real_pos / CHAR_BIT)] &= ~_BITX(real_pos % CHAR_BIT);
		return *this;
	}

	// vvv Methods to match std::deque vvv
	deque& operator=(const deque& other) {

	}
	deque& operator=(deque&& other) noexcept {

	}
	deque& operator=(std::initializer_list<bool> ilist) {

	}







public:
	~deque() { delete[] carr_; }
private:



	size_type _Getblock(size_type _Off) const noexcept {
		return _Get_data()._Getblock(_Off);
	}

	void _Orphan_all() noexcept {
		_Get_data()._Orphan_all();
	}

	_Alty& _Getal() noexcept {
		return _Mypair._Get_first();
	}

	const _Alty& _Getal() const noexcept {
		return _Mypair._Get_first();
	}

	_Scary_val& _Get_data() noexcept {
		return _Mypair._Myval2;
	}

	const _Scary_val& _Get_data() const noexcept {
		return _Mypair._Myval2;
	}

	_Mapptr& _Map() noexcept {
		return _Get_data()._Map;
	}

	const _Mapptr& _Map() const noexcept {
		return _Get_data()._Map;
	}

	size_type& _Mapsize() noexcept {
		return _Get_data()._Mapsize;
	}

	const size_type& _Mapsize() const noexcept {
		return _Get_data()._Mapsize;
	}

	size_type& _Myoff() noexcept {
		return _Get_data()._Myoff;
	}

	const size_type& _Myoff() const noexcept {
		return _Get_data()._Myoff;
	}

	size_type& _Mysize() noexcept {
		return _Get_data()._Mysize;
	}

	const size_type& _Mysize() const noexcept {
		return _Get_data()._Mysize;
	}

	_Compressed_pair<_Alty, _Scary_val> _Mypair;
};

_NSTD_END
#endif // !_NSTD_DEQUE_