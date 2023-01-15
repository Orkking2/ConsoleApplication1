#pragma once
#ifndef _NSTD_MATRIX_
#define _NSTD_MATRIX_

#include <xmemory>
#include "Defines.h"
#include "TypeTraits.h"

_NSTD_BEGIN

template <typename _Ty, typename _Alloc = _STD allocator<_Ty>>
class Matrix {
	using _Alty			= _STD _Rebind_alloc_t<_Alloc, _Ty>;
	using _Alty_traits	= _STD allocator_traits<_Alty>;
	using _Alpty		= _STD _Rebind_alloc_t<_Alloc, typename _Alty_traits::pointer>;
	using _Alpty_traits = _STD allocator_traits<_Alpty>;
	using _Mapptr		= typename _Alpty_traits::pointer;


public:
	Matrix(_Mapptr init, uint x, uint y) : _X(x), _Y(y) {

	}



private:

	_NODISCARD _Mapptr static _Generate(uint x, uint y) {

	}


	_Map_ptr_t _Under;
	_NSTD uint _X, _Y;
};


// TODO (may be difficult)
template <typename _Ty, size_t _Dims = 2, typename _Alloc = _STD allocator<_Ty>>
class NDMatrix {
	constexpr bool _Base_case() { return _Dims == 1; }
	constexpr size_t _Min(size_t a, size_t b) { return a < b ? a : b; }

	using _Under_t		= _STD conditional_t<_Base_case(), _Ty, NDMatrix<_Ty, _Dims - 1, _Alloc>>;
	using _Alty			= _STD _Rebind_alloc_t<_Alloc, _Under_t>;
	using _Alty_traits	= _STD allocator_traits<_Alty>;
	using _Myptr_t		= typename _Alty_traits::pointer;

	template <typename, size_t, typename> 
	friend class Matrix;

public:

//	Commented because no return statement	
//	template <size_t n>
//	NDMatrix<T, _Min(_Dims, n)> multiply(const NDMatrix<T, n>& other) const {
//		_NSTD_ASSERT(_Size == other._Size,
//			"Incompatible sizes for matrix multiplication");
//
//		
//	}

	_Under_t& operator[] (const _NSTD uint& _Index) {
		_NSTD_ASSERT(_Index < _Size,
			"_Index out of bounds");
		return _Under[_Index];
	}

private:


	_Myptr_t _Under;
	uint _Size;
};








_NSTD_END
#endif // _NSTD_MATRIX_