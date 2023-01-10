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

	uint rp_;
	uchar* char_;
public:
	_Bproxy(uchar* c, uint rp) : rp_(rp), char_(c) {}
	_Bproxy(uchar& c, uint rp) : rp_(rp), char_(&c) {}
	_Bproxy(_Bproxy& other) : rp_(other.rp_), char_(other.char_) {}
	// funcs
	_Bproxy& set(bool b) {
		b ? *char_ |=  _BITX(rp_)
		  : *char_ &= ~_BITX(rp_);
		return *this;
	}
	// ops
	operator bool() {
		return *char_ & _BITX(rp_);
	}
	operator const bool() const {
		return *char_ & _BITX(rp_);
	}
	bool operator* () {
		return static_cast<bool>(*this);
	}
	const bool operator* () const {
		return static_cast<const bool>(*this);
	}
	// One cannot access internal bools via classic pointers
	// This class should in itself be used as a pointer
	bool* operator& () = delete;
	_Bproxy& operator= (bool b) {
		return set(b);
	}
	_Bproxy& operator= (const _Bproxy& other) {
		rp_ = other.rp_;
		char_ = other.char_;
		return *this;
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
	// Booleans have no -> op functionality
	// Note: same is true for _Bproxy
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
		_STL_VERIFY(_Mycont->_Myhead <= this->_Myoff && this->_Myoff < _Mycont->_Mytail,
			"cannot deference out of range deque iterator");
#endif // _ITERATOR_DEBUG_LEVEL != 0

		_Size_type _Block = _Mycont->_Getblock(_Myoff);
		_Size_type _Off   = _Myoff % _Block_size;
		return reference(_Mycont->_Map[_Block][_Off / CHAR_BIT], _Off % CHAR_BIT);
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
				_Mycont->_Myhead <= this->_Myoff + _Off && this->_Myoff + _Off <= _Mycont->_Mytail,
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

	/*_NODISCARD reference operator*() const noexcept {
		return const_cast<reference>(_Mybase::operator*());
	}*/

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
struct _Db_val : _STD _Container_base12 {
	using value_type      = typename _Val_types::value_type;
	using size_type       = typename _Val_types::size_type;
	using difference_type = typename _Val_types::difference_type;
	using pointer         = typename _Val_types::pointer;
	using const_pointer   = typename _Val_types::const_pointer;
	using reference       = typename _Val_types::reference;
	using const_reference = typename _Val_types::const_reference;
	using _Mapptr         = typename _Val_types::_Mapptr;

	static constexpr int _Block_size_RAW = 16;                         // 16 bytes per block
	static constexpr int _Block_size     = _Block_size_RAW * CHAR_BIT; // 128 bits per block

	_Db_val() noexcept : _Map(), _Mapsize(0), _Myhead(1), _Mytail(0) {}

	size_type _Getblock(size_type _Off) const noexcept {
		return (_Off / _Block_size) % _Mapsize;
	}

	_Mapptr _Map; // pointer to array of pointers to blocks
	size_type _Mapsize; // size of map array
	size_type _Myhead; // offset of initial element
	size_type _Mytail; // current length of sequence
};


// This is a class optimized specifically for use in huffman encoding
// See: Huff.h & Huff.cpp
template <typename _Alloc>
class deque<bool, _Alloc> {
	_NSTD_UNSIGNED
	friend _STD _Tidy_guard<deque>;
	using _Ty = bool;

private:
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

	static constexpr int _Block_size_RAW = _Scary_val::_Block_size_RAW;
	static constexpr int _Block_size     = _Scary_val::_Block_size;

	enum SELECTOR {
		FORWARD = 0,
		BACKWARD,
	};

public:
	using allocator_type  = _Alloc;
	using value_type      = _Ty;
	using storage_type    = typename _Alty_traits::value_type;
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
	deque() : _Mypair(_STD _Zero_then_variadic_args_t{}) {
		_Get_data()._Alloc_proxy(static_cast<_Alproxy_ty>(_Getal()));
	}

	explicit deque(const _Alloc& _Al) : _Mypair(_STD _One_then_variadic_args_t{}, _Al) {
		_Get_data()._Alloc_proxy(static_cast<_Alproxy_ty>(_Getal()));
	}
	
	deque(const deque& _Right)
		: _Mypair(_STD _One_then_variadic_args_t{}, _Alty_traits::select_on_container_copy_construction(_Right._Getal())) {
		_Alproxy_ty _Alproxy(_Getal());
		_STD _Container_proxy_ptr12<_Alproxy_ty> _Proxy(_Alproxy, _Get_data());
		_Construct(_Right._Unchecked_begin(), _Right._Unchecked_end());
		_Proxy._Release();
	}
	
/*	deque(uint size = 0xff) : carr_(new uchar[size / CHAR_BIT + 1]), carr_sz_(size / CHAR_BIT + 1), Hpos_(carr_sz_* CHAR_BIT / 2), Tpos_(Hpos_ - 1) {}
	deque(const deque& other) : carr_(new uchar[other.carr_sz_]),      carr_sz_(other.carr_sz_),      Hpos_(other.Hpos_),  Tpos_(other.Tpos_) {
		_NSTD_FOR_I(carr_sz_)
			carr_[_I] = other.carr_[_I];
	}*/
	
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


	// Inclusive
	_NODISCARD size_type size() {
		return _Mysize();
	}
	_NODISCARD size_type real_size() {
		return _Mapsize() * _Block_size;
	}

	_NODISCARD bool empty() {
		return size() == 0;
	}
	// Maintains size
	void clear() = delete;
	// Copies contents of old deque into new deque (maintains position)
	// Note: expands backwards
	void resize(uint, bool) = delete;
	void resize_NOCOPY(uint num_bools, bool init = false) {
		
	}
	_NODISCARD reference front() {
		_NSTD_ASSERT(!empty(), "called front() on empty deque");
		return *begin();
	}
	_NODISCARD const_reference front() const {
		_NSTD_ASSERT(!empty(), "called front() on empty deque");
		return *begin();
	}
	reference pop_front() {
		reference out(front());
		++_Myhead();
		return out;
	}
	_NODISCARD reference back() {
		_NSTD_ASSERT(!empty(), "called back() on empty deque");
		return *end();
	}
	_NODISCARD const_reference back() const {
		_NSTD_ASSERT(!empty(), "called back() on empty deque");
		return *end();
	}
	reference pop_back() {
		reference out(back());
		--_Mytail();
		return out;
	}
	// Combination of shrink_front & shrink_back
	deque& shrink_fit() {
		return shrink_front().shrink_back();
	}
	deque& shrink_front() {
		
	}
	deque& shrink_back() {
		
	}
	deque& push_back(bool b) {
		if (!(++_Mytail() % _Block_size) && !(real_size() - _Mytail())) {
			_Growmap(BACKWARD, 1);
		}
		set(_Mytail() - _Myhead(), b);
	}
	deque& emplace_back(bool&& b) {
		push_back(b);
	}
	deque& emplace_back(bool& b) {
		push_back(b);
	}

	deque& push_front(bool b) {
		if (!--_Myhead()) {
			_Growmap(FORWARD, 1);
		}
		set(0, b);
	}
	deque& emplace_front(bool&& b) {
		return push_front(b);
	}
	deque& emplace_front(bool& b) {
		return push_front(b);
	}
		
	_NODISCARD size_type size() const noexcept {
		return _Mysize();
	}

	_NODISCARD size_type max_size() const noexcept {
		return (_STD min) (static_cast<size_type>((numeric_limits<difference_type>::max) ()),
			_Alty_traits::max_size(_Getal()));
	}

	_NODISCARD bool empty() const noexcept {
		return _Mysize() == 0;
	}

	_NODISCARD allocator_type get_allocator() const noexcept {
		return static_cast<allocator_type>(_Getal());
	}

	_NODISCARD const_reference at(size_type _Pos) const {
		if (_Mysize() <= _Pos) {
			_Xran();
		}

		return *(begin() + static_cast<difference_type>(_Pos));
	}
	_NODISCARD const_reference operator[] (size_type pos) const {
		return at(pos);
	}

	_NODISCARD reference at(size_type _Pos) {
		if (_Mysize() <= _Pos) {
			_Xran();
		}

		return *(begin() + static_cast<difference_type>(_Pos));
	}
	_NODISCARD reference operator[] (size_type pos) {
		return at(pos);
	}

	// set(pos, !at(pos))
	deque& flip(uint pos) {
		return set(pos, !at(pos));
	}
	deque& set(uint pos, bool b = true) {
		_NSTD_ASSERT(_Myhead() + pos <= _Mytail(), "tried to access deque element outside deque bounds");
		at(pos) = b;
		return *this;
	}

	





	_NODISCARD iterator begin() noexcept {
		return iterator(_Myhead(), _STD addressof(_Get_data()));
	}

	_NODISCARD const_iterator begin() const noexcept {
		return const_iterator(_Myhead(), _STD addressof(_Get_data()));
	}

	_NODISCARD iterator end() noexcept {
		return iterator(_Mytail(), _STD addressof(_Get_data()));
	}

	_NODISCARD const_iterator end() const noexcept {
		return const_iterator(_Mytail(), _STD addressof(_Get_data()));
	}

	_Unchecked_iterator _Unchecked_begin() noexcept {
		return _Unchecked_iterator(_Myhead(), _STD addressof(_Get_data()));
	}

	_Unchecked_const_iterator _Unchecked_begin() const noexcept {
		return _Unchecked_const_iterator(_Myhead(), _STD addressof(_Get_data()));
	}

	_Unchecked_iterator _Unchecked_end() noexcept {
		return _Unchecked_iterator(_Mytail(), _STD addressof(_Get_data()));
	}

	_Unchecked_const_iterator _Unchecked_end() const noexcept {
		return _Unchecked_const_iterator(_Mytail(), _STD addressof(_Get_data()));
	}

	iterator _Make_iter(const_iterator _Where) const noexcept {
		return iterator(_Where._Myoff, _STD addressof(_Get_data()));
	}

	_NODISCARD reverse_iterator rbegin() noexcept {
		return reverse_iterator(end());
	}

	_NODISCARD const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator(end());
	}

	_NODISCARD reverse_iterator rend() noexcept {
		return reverse_iterator(begin());
	}

	_NODISCARD const_reverse_iterator rend() const noexcept {
		return const_reverse_iterator(begin());
	}

	_NODISCARD const_iterator cbegin() const noexcept {
		return begin();
	}

	_NODISCARD const_iterator cend() const noexcept {
		return end();
	}

	_NODISCARD const_reverse_iterator crbegin() const noexcept {
		return rbegin();
	}

	_NODISCARD const_reverse_iterator crend() const noexcept {
		return rend();
	}

public:
	~deque() { _Tidy(); }
private:

	void _Tidy() noexcept { // free all storage
		_Orphan_all();

		_Alpty _Almap(_Getal());
		while (!empty()) {
			pop_back();
		}

		for (size_type _Block = _Mapsize(); 0 < _Block;) { // free storage for a block and destroy pointer
			if (_Map()[--_Block]) { // free block and destroy its pointer
				_Getal().deallocate(_Map()[_Block], _Block_size);
				_STD _Destroy_in_place(_Map()[_Block]);
			}
		}

		if (_Map() != _Mapptr()) {
			_Almap.deallocate(_Map(), _Mapsize()); // free storage for map
		}

		_Mapsize() = 0;
		_Map()     = _Mapptr();
	}

	template <class _Iter>
	void _Construct(_Iter _First, _Iter _Last) { // initialize from [_First, _Last), input iterators
		_STD _Tidy_guard<deque> _Guard{ this };
		for (; _First != _Last; ++_First) {
			emplace_back(bool(*_First));
		}

		_Guard._Target = nullptr;
	}

	void _Take_contents(deque& _Right) noexcept {
		// move from _Right, stealing its contents
		// pre: _Getal() == _Right._Getal()
		auto& _My_data = _Get_data();
		auto& _Right_data = _Right._Get_data();
		_My_data._Swap_proxy_and_iterators(_Right_data);
		_My_data._Map = _Right_data._Map;
		_My_data._Mapsize = _Right_data._Mapsize;
		_My_data._Myhead = _Right_data._Myhead;
		_My_data._Mytail = _Right_data._Mytail;

		_Right_data._Map = nullptr;
		_Right_data._Mapsize = 0;
		_Right_data._Myhead = 1;
		_Right_data._Mytail = 0;
	}

	void _Growmap(SELECTOR s, size_type num) {
		switch (s) {
		case FORWARD:

			break;
		case BACKWARD:

			break;
		default:
			exit(-1);
		}
	}

	[[noreturn]] void _Xlen() const {
		_STD _Xlength_error("deque<T> too long");
	}
	[[noreturn]] void _Xran() const {
		_STD _Xout_of_range("invalid deque<T> subscript");
	}

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

	size_type& _Myhead() noexcept {
		return _Get_data()._Myhead;
	}

	const size_type& _Myhead() const noexcept {
		return _Get_data()._Myhead;
	}

	size_type& _Mytail() noexcept {
		return _Get_data()._Mytail;
	}

	const size_type& _Mytail() const noexcept {
		return _Get_data()._Mytail;
	}

	const size_type _Mysize() noexcept {
		return _Get_data()._Mytail - _Get_data()._Myhead + 1;
	}

	const size_type& _Mysize() const noexcept {
		return _Get_data()._Mytail - _Get_data()._Myhead + 1;
	}

	_STD _Compressed_pair<_Alty, _Scary_val> _Mypair;
};

_NSTD_END
#endif // !_NSTD_DEQUE_