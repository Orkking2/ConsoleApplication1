#pragma once
#ifndef _NSTD_ARRAYUNDER_
#define _NSTD_ARRAYUNDER_

#include <memory>
#include "Defines.h"
#include "Pair.h"

_NSTD_BEGIN

template <typename _Ty, typename _Alloc>
class _Contiguous_container {
	friend _STD _Tidy_deallocate_guard<_Contiguous_container>;

	using _Alty        = _STD _Rebind_alloc_t<_Alloc, _Ty>;
	using _Alty_traits = _STD allocator_traits<_Alty>;
	using _Mysize_t    = typename _Alty::size_type;
	using _Myunder_t   = typename _Alty_traits::value_type;
	using _Myptr_t     = typename _Alty_traits::pointer;

public:
	using value_type		= _Myunder_t;
	using allocator_type	= _Alty;
	using size_type			= _Mysize_t;
	using difference_type	= typename _Alty_traits::difference_type;
	using reference			= value_type&;
	using const_reference	= const value_type&;
	using pointer			= _Myptr_t;
	using const_pointer		= typename _Alty_traits::const_pointer;
//  TODO
//	using iterator = 
//	using const_iterator =
//	using reverse_iterator =
//	using const_reverse_iterator =

public:
	void grow(const size_type& _Newsize, const_pointer _Ref = 0, size_type _Oldize = 0) {
		_NSTD_ASSERT(_Newsize >= 0,
			"Allocating negative memory?");

	}



protected:
	_Mysize_t& _Mysize() {
		return _Mypair.first;
	}

	// Unchecked
	_Myptr_t& _Myarr() {
		return _Mypair.second;
	}
private:
	void _Tidy_deallocate() {
		_Alty alloc;
		alloc.deallocate(_Myarr(), _Mysize());
	}

	_NSTD pair<_Mysize_t, _Myptr_t> _Mypair;
};

//template <typename T>
//using _ArrayUnder = _ArrayUnder<typename T::value_type, typename T::allocator_type>;








_NSTD_END
#endif // _NSTD_ARRAYUNDER_