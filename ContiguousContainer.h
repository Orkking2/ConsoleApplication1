#pragma once
#ifndef _NSTD_ARRAYUNDER_
#define _NSTD_ARRAYUNDER_

#include <xmemory>
#include "Defines.h"
#include "Pair.h"

_NSTD_BEGIN

template <typename _Ty, typename _Alloc>
class _Contiguous_container {
	friend _STD _Tidy_deallocate_guard<_Contiguous_container>;
	template <typename, typename> friend class _Contiguous_containter;

	using _Alty			= _STD _Rebind_alloc_t<_Alloc, _Ty>;
	using _Alty_traits	= _STD allocator_traits<_Alty>;

protected:
	using value_type		= typename _Alty_traits::value_type;
	using allocator_type	= _Alty;
	using size_type			= typename _Alty_traits::size_type;
	using difference_type	= typename _Alty_traits::difference_type;
	using reference			= value_type&;
	using const_reference	= const value_type&;
	using pointer			= typename _Alty_traits::pointer;
	using const_pointer		= typename _Alty_traits::const_pointer;

//  TODO
//	using iterator = 
//	using const_iterator =
//	using reverse_iterator =
//	using const_reverse_iterator =

	using pair_type = _NSTD pair<size_type, pointer>;

private:
	static constexpr size_type	_Mybytesize	= sizeof(value_type);
	static constexpr bool		_Copyable	= _STD is_trivially_copy_constructible_v<value_type>;

protected:
		template <typename st1, typename st2>
		_NODISCARD static constexpr auto const& _Min(const st1& first, const st2& second) {
			return first > second ? second : first;
		}

protected:
	_Contiguous_container()											: _Mypair({ 0, 0 }) {}
	_Contiguous_container(const size_type& size)					: _Mypair({ 0, 0 }) { _Grow(size); }
	_Contiguous_container(const _Contiguous_container& other)		: _Mypair({ 0, 0 }) { _Set(other); }
	_Contiguous_container(const_pointer _Ref, size_type _Refsize)	: _Mypair({ 0, 0 }) { _Grow(_Refsize, _Ref, _Refsize); }

	~_Contiguous_container() { _Tidy_deallocate(); }

protected:
	auto& operator[] (const size_type& index) const {
		_NSTD_ASSERT(index < _Mysize(),
			"index out of bounds");
		return _Myarr()[index];
	}

protected:
	void _Grow(const size_type& _Newsize, const_pointer _Ref, size_type _Refsize) {
		//_NSTD_ASSERT(_Newsize >= 0,
		//	"Allocating negative memory?");
		// vvv
		// Assume size_type is unsigned or _Alloc knows how to allocate negative memory

		_Tidy_deallocate();

		_Myarr() = _Alloc(_Newsize);
		_Mysize() = _Newsize;
		_Construct_default(alloc);
		_Construct(_Ref, _Refsize);
	}

	// Forward _Alty::allocate(_Size)
	pointer _Alloc(const size_type& _Size) {
		_Alty alloc;
		return alloc.allocate(_Size);
	}

	_STD enable_if_t<_Copyable> _Construct(const_pointer _Ref, size_type _Refsize) {
		_NSTD_FOR_I(_Min(_Refsize, _Mysize()))
			_Myarr()[_I] = _Ref[_I];
	}


	_STD enable_if_t<_Copyable, _Contiguous_container> _Copy() const {
		_Contiguous_container c;
		c._Grow(_Mysize(), _Myarr(), _Mysize());
		return c;
	}

	void _Grow(const size_type& _Newsize) {
		_Contiguous_container c(_Copy());
		_Tidy_deallocate();
		_Grow(_Newsize, c._Myarr(), c._Mysize());
	}


	//_STD enable_if_t<!_Copyable> _Set(const _Contiguous_container&) = delete;
	_STD enable_if_t<_Copyable> _Set(const _Contiguous_container& other) {
		_Grow(other._Mysize(), other._Myarr(), other._Mysize());
	}

	void _Tidy_deallocate() {
		_Alty alloc;
		alloc.deallocate(_Myarr(), _Mysize());
		_Myarr() = nullptr;
		_Mysize() = 0;
	}

	size_type& _Mysize() {
		return _Mypair.first;
	}
	const size_type& _Mysize() const {
		return _Mypair.first;
	}

	// Unchecked
	pointer& _Myarr() {
		return _Mypair.second;
	}
	const pointer& _Myarr() const {
		return _Mypair.second;
	}

private:
	//_STD enable_if_t<!_STD is_default_constructible_v<value_type>> _Construct_default(_Alty) {}
	_STD enable_if_t< _STD is_default_constructible_v<value_type>> _Construct_default(_Alty alloc) {
		_NSTD_FOR_I(_Mysize())
			_Alty_traits::construct(alloc, (_Myarr() + _I), value_type());
	}

	void _Grow_RAWCOPY(const size_type& _Newsize, const_pointer _Ref, size_type _Refsize) {
		_Tidy_deallocate();
		_Myarr() = _Alloc(_Newsize);
		_Mysize() = _Newsize;
		_Construct_default();
		_Construct_RAWCOPY(_Ref, _Refsize);
	}

	void _Construct_RAWCOPY(const_pointer _Ref, size_type _Refsize) {
		_NSTD_FOR_I(_Min(_Refsize * _Mybytesize, _Mysize() * _Mybytesize))
			reinterpret_cast<char*>(_Myarr())[_I] = reinterpret_cast<const char*>(_Ref)[_I];
	}


	pair_type _Mypair;
};

//template <typename T>
//using _ArrayUnder = _ArrayUnder<typename T::value_type, typename T::allocator_type>;








_NSTD_END
#endif // _NSTD_ARRAYUNDER_