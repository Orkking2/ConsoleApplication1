#pragma once
#ifndef _NSTD_ARRAYUNDER_
#define _NSTD_ARRAYUNDER_

#include <memory>
#include "Defines.h"
#include "Pair.h"

_NSTD_BEGIN

template <typename _Ty, typename _Alloc>
struct _Contiguous_container_vals {
	using _Alty        = _STD _Rebind_alloc_t<_Alloc, _Ty>;
	using _Alty_traits = _STD allocator_traits<_Alty>;

	using value_type		= typename _Alty_traits::value_type;
	using allocator_type	= _Alty;
	using size_type			= typename _Alty::size_type;
	using difference_type	= typename _Alty_traits::difference_type;
	using reference			= value_type&;
	using const_reference	= const value_type&;
	using pointer			= typename _Alty_traits::pointer;
	using const_pointer		= typename _Alty_traits::const_pointer;

	using pair_type = _NSTD pair<size_type, pointer>;
};


template <typename _Ty, typename _Alloc>
class _Contiguous_container {
	friend _STD _Tidy_deallocate_guard<_Contiguous_container>;
	template <typename, typename> friend class _Contiguous_containter;

	using _Scary_val	= _Contiguous_container_vals<_Ty, _Alloc>;
	using _Alty			= _Scary_val::_Alty;
	using _Alty_traits	= _Scary_val::_Alty_traits;
	using _Mysize_t		= _Scary_val::size_type;
	using _Myunder_t	= _Scary_val::value_type;
	using _Myptr_t		= _Scary_val::pointer;

public:
	template <typename st1, typename st2>
	_NODISCARD static constexpr auto const& _Min(const st1& first, const st2& second) {
		return first > second ? second : first;
	}

	using _Mypair_t = _Scary_val::pair_type;

public:
	using value_type		= _Scary_val::value_type;
	using allocator_type	= _Scary_val::allocator_type;
	using size_type			= _Scary_val::size_type;
	using difference_type	= _Scary_val::difference_type;
	using reference			= _Scary_val::reference;
	using const_reference	= _Scary_val::const_reference;
	using pointer			= _Scary_val::pointer;
	using const_pointer		= _Scary_val::const_pointer;
//  TODO
//	using iterator = 
//	using const_iterator =
//	using reverse_iterator =
//	using const_reverse_iterator =

public:
	_Contiguous_container()											: _Mypair({ 0, 0 }) {}
	_Contiguous_container(const _Contiguous_container& other)		: _Mypair({ 0, 0 }) { _Set(other); }
	_Contiguous_container(const_pointer _Ref, size_type _Refsize)	: _Mypair({ 0, 0 }) { _Grow(_Refsize, _Ref, _Refsize); }

	~_Contiguous_container() { _Tidy_deallocate(); }

public:
	void _Grow(const size_type& _Newsize, const_pointer _Ref, size_type _Refsize) {
		//_NSTD_ASSERT(_Newsize >= 0,
		//	"Allocating negative memory?");
		// vvv
		// Assume size_type is unsigned or _Alloc knows how to allocate negative memory

		_Alty alloc;
		_Alty_traits::deallocate(alloc, _Myarr(), _Mysize());
		_Myarr() = alloc.allocate(_Newsize);
		_NSTD_FOR_I(_Newsize)
			_Alty_traits::construct(alloc, (_Myarr() + _I), _Myunder_t());
		_NSTD_FOR_I(_Min(_Refsize, _Newsize))
			_Myarr()[_I] = _Ref[_I];
		_Mysize() = _Newsize;
	}

	_Contiguous_container _Copy() const {
		_Contiguous_container c;
		c._Grow(_Mysize(), _Myarr(), _Mysize());
		return c;
	}

	void _Grow(const size_type& _Newsize) {
		_Contiguous_container c(_Copy());
		_Tidy_deallocate();
		_Grow(_Newsize, c._Myarr(), c._Mysize());
	}

	void _Set(const _Contiguous_container& other) {
		_Grow(other._Mysize(), other._Myarr(), other._Mysize());
	}

	void _Tidy_deallocate() {
		_Alty alloc;
		alloc.deallocate(_Myarr(), _Mysize());
	}

	_Mysize_t& _Mysize() {
		return _Mypair.first;
	}
	const _Mysize_t& _Mysize() const {
		return _Mypair.first;
	}

	// Unchecked
	_Myptr_t& _Myarr() {
		return _Mypair.second;
	}
	const _Myptr_t& _Myarr() const {
		return _Mypair.second;
	}

private:
	_Mypair_t _Mypair;
};

//template <typename T>
//using _ArrayUnder = _ArrayUnder<typename T::value_type, typename T::allocator_type>;








_NSTD_END
#endif // _NSTD_ARRAYUNDER_