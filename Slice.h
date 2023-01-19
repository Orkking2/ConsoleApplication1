#pragma once
#ifndef _NSTD_SLICE_
#define _NSTD_SLICE_

#include <concepts>
#include "Defines.h"
#include "TypeTraits.h"

_NSTD_BEGIN

template <typename _Ty>
concept _Good_under = requires(_Ty t) {
	{ t.size() } -> _STD same_as<typename T::size_type>;
	{ t.begin() } -> _STD same_as<_STD conditional_t<_STD is_const_v<T>, typename T::const_iterator, typename T::iterator>>;
	{ t.begin() + _STD declval<typename T::size_type>() } -> _STD same_as<_STD conditional_t<_STD is_const_v<T>, typename T::const_iterator, typename T::iterator>>;
};

template <typename T>
	//requires _Good_under<T>
struct slice {
	using _Array_t = T;

	slice(T& _Under, typename T::size_type _Off, typename T::size_type _Size)
		: _Under(_Under), _Off(_Off), _Size(_Size) {}

	slice(const slice& other, typename T::size_type _Off, typename T::size_type _Size)
		: _Under(other._Under), _Off(_Off + other._Off), _Size(_Size) {}


	_STD conditional_t<_STD is_const_v<T>, typename T::const_iterator, typename T::iterator> begin() { return _Under.begin() + _Off; }
	typename T::size_type size() { return _Size; }

	T& _Under;
	typename T::size_type _Off;
	typename T::size_type _Size;
};


_NSTD_END
#endif // _NSTD_SLICE_