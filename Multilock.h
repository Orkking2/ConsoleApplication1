#pragma once
#ifndef _NSTD_MULTILOCK_
#define _NSTD_MULTILOCK_

#include <synchapi.h>
#include "Defines.h"
#include "ContiguousContainer.h"


_NSTD_BEGIN

// SRWLOCK wrapper
class _LWMtx_t {
public:
	_LWMtx_t() {
		InitializeSRWLock(&_SRWLock);
	}

	void lock() {
		AcquireSRWLockExclusive(&_SRWLock);
	}

	bool try_lock() {
		return TryAcquireSRWLockExclusive(&_SRWLock) != 0;
	}

	void unlock() {
		ReleaseSRWLockExclusive(&_SRWLock);
	}

	PSRWLOCK native_handle() {
		return &_SRWLock;
	}
	
	_LWMtx_t(const _LWMtx_t&)				= delete;
	_LWMtx_t& operator=(const _LWMtx_t&)	= delete;

private:
	SRWLOCK _SRWLock;
};



class Multilock : _NSTD _Contiguous_container<_LWMtx_t, _STD allocator<_LWMtx_t>> {
	using _Base = _NSTD _Contiguous_container<_LWMtx_t, _STD allocator<_LWMtx_t>>;

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
	Multilock(const Multilock&)			= delete; // _LWMtx_t uncopyable
	Multilock& operator=(const Multilock&) = delete; // _LWMtx_T uncopyable

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