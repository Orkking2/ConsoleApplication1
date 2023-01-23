#pragma once
#ifndef _NSTD_MULTILOCK_
#define _NSTD_MULTILOCK_

#include "Defines.h"
#include "ContiguousContainer.h"


_NSTD_BEGIN

template <typename _Storage_t = uint, typename _Alloc = _STD allocator<_Storage_t>>
class Multilock : _NSTD _Contiguous_container<_Storage_t, _STD allocator<_Storage_t>> {
	using _Base = _NSTD _Contiguous_container<_Storage_t, _STD allocator<_Storage_t>>;

public:
	using storage_type		= _Base::value_type;
	using allocator_type	= _Base::allocator_type;
	using size_type			= _Base::size_type;
	using difference_type	= _Base::difference_type;
//	using reference			= _Base::reference;
//	using const_reference	= _Base::const_reference;
//	using pointer			= _Base::pointer;
//	using const_pointer		= _Base::const_pointer;

private:
	static constexpr size_type _Mybytesize = sizeof(storage_type);
	static constexpr size_type _Mybitsize  = _Mybytesize * CHAR_BIT;

public:
	Multilock()							: _Base() {}
	Multilock(const size_type& size)	: _Base(size) {}
	Multilock(const Multilock& other)	: _Base(other) {}

public:
	void lock(const size_type& index) {

	}
	_NODISCARD bool try_lock(const size_type& index) {

	}








private:
	void _Grow(size_type _Newsize) {
		_Base::_Grow(_Newsize);
	}

	auto& _Mysize() const {
		return _Base::_Mysize();
	}
	auto& _Myarr() const {
		return _Base::_Myarr();
	}
};








_NSTD_END
#endif // _NSTD_MULTILOCK_