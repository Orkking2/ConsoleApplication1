#pragma once
#ifndef _NSTD_MATRIX_
#define _NSTD_MATRIX_

#include <xmemory>
#include "Defines.h"
#include "ContiguousContainer.h"
#include "TypeTraits.h"
#include "Threads.h"

_NSTD_BEGIN

// TODO vvv make comment vvv
#define _NSTD_MATRIX_MULTITHREAD_

//#ifdef _NSTD_MATRIX_MULTITHREAD_

//#endif

template <typename _Ty, typename _Alloc = _STD allocator<_Ty>>
class Matrix : _NSTD _Contiguous_container<_Ty, _Alloc> {
	using _Base = _NSTD _Contiguous_container<_Ty, _Alloc>;

public:
	using storage_type = _Base::value_type;
	using allocator_type = _Base::allocator_type;
	using size_type = _Base::size_type;
	using difference_type = _Base::difference_type;
	//	using reference			= _Base::reference;
	//	using const_reference	= _Base::const_reference;
	//	using pointer			= _Base::pointer;
	//	using const_pointer		= _Base::const_pointer;

private:
	static constexpr size_t _Max(size_t a, size_t b, size_t c) {
		return 
			a > b ?
				a > c ?
					a :
				c :
			b > c ?
				b :
			c;
	}

public:
	Matrix(const uint& row, const uint& col) {
		
	}
	Matrix(const uint& row, const uint& col) {
		
	}
	Matrix(const Matrix& other) {
		
	}
	



	_Ty& get(const uint& row, const uint& col) {
		return ;
	}
	const _Ty& get(const uint& row, const uint& col) const {
		return ;
	}

	Matrix& operator= (const Matrix& other) {
		
		return *this;
	}

private:
	

	void _Add_check(const Matrix& other) {
		_NSTD_ASSERT(other._Mapsize == _Mapsize && other._Undersize == _Undersize,
			"Adding matrices of non-similar sizes");
	}
	Matrix _Simple_iterative_add_unchecked(const Matrix& other) const {
		Matrix out(_Mapsize, _Undersize);
		_NSTD_FOR_I(_Mapsize)
			_NSTD_FOR_J(_Undersize)
				out.get(_I, _J) = get(_I, _J) + other.get(_I, _J);
		return out;
	}
	void _Multiply_check(const Matrix& other) {
		_NSTD_ASSERT(_Undersize == other._Mapsize,
			"Multiplying matrices of non-similar sizes, consider changing the order of multiplication");
	}
	Matrix _Simple_iterative_multiply_unchecked(const Matrix& other) const {
		Matrix out(_Mapsize, other._Undersize);
		_NSTD_FOR_I(out._Mapsize) {
			_NSTD_FOR_J(out._Undersize) {
				out.get(_I, _J) = 0;
				_NSTD_FOR_K(_Undersize /* = other._Mapsize */)
					out.get(_I, _J) += get(_I, _K) * other.get(_K, _J);
			}
		}
		return out;
	}
#ifdef _NSTD_MATRIX_MULTITHREAD_ // THREADED METHODS
	// Lightweight slice for Matrix
	struct Partition {
		_Mapptr_t _Map;
		uint _RSize, _ROff,
			 _CSize, _COff;

		// Unchecked, never call get() outside _NSTD_FOR_I(Partition::_RSize)... etc
		_Ty& get(size_t _Row, size_t _Col) {
			return _Map[_Row + _ROff][_Col + _COff];
		}
	};

	Partition _Partition() {
		return Partition{ _Mapptr, _Mapsize, 0, _Undersize, 0 }
	}

	operator Partition() {
		return _Partition();
	}

	// Partition methods -- seperate from Partition object to save space
	static Partition _LHSplit(Partition& p) {
		return Partition{ p._Map, p._RSize, p._ROff, p._CSize / 2, p._COff };
	}
	static Partition _RHSplit(Partition& p) {
		return Partition{ p._Map, p._RSize, p._ROff, p._CSize - p._CSize / 2, p._COff + p._COff / 2 };
	}
	static Partition _TVSplit(Partition& p) {
		return Partition{ p._Map, p._RSize / 2, p._ROff, p._CSize, p._COff };
	}
	static Partition _BVSplit(Partition& p) {
		return Partition{ p._Map, p._RSize - p._RSize / 2, p._ROff + p._ROff / 2, p._CSize, p._COff };
	}

	static void _PAdd_check(Partition& _Ret, const Partition& _Left, const Partition& _Right) {
		_NSTD_ASSERT(_Ret._RSize == _Left._Undersize && _Ret._RSize == _Right._RSize &&
			_Ret._CSize == _Left._CSize && _Ret._CSize == _Right._CSize,
				"Adding partitions of non-similar sizes");
	}
	static void _PSimple_iterative_add_unchecked(Partition& _Ret, const Partition& _Left, const Partition& _Right) {
		_NSTD_FOR_I(_Ret._RSize)
			_NSTD_FOR_J(_Ret._CSize)
				_Ret.get(_I, _J) = _Left.get(_I, _J) + _Right.get(_I, _J);
	}

	static void _PMultiply_check(Partition& _Ret, const Partition& _Left, const Partition& _Right) {
		_NSTD_ASSERT(_Ret._RSize == _Left._RSize && _Ret._CSize == _Right._CSize && _Left._CSize == _Right._RSize,
			"Multiplying partitions of non-similar sizes");
	}
	static void _PSimple_iterative_multiply_unchecked(Partition& _Ret, const Partition& _Left, const Partition& _Right) {
		_NSTD_FOR_I(_Ret._RSize) {
			_NSTD_FOR_J(_Ret._CSize) {
				_Ret.get(_I, _J) = 0;
				_NSTD_FOR_K(_Left._CSize /* = _Right._RSize */)
					_Ret.get(_I, _J) += _Left.get(_I, _K) * _Right.get(_K, _J);
			}
		}
	}

	// _Ret <- A1B1 + A2B2
	static void _PMA(Partition& _Ret, const Partition& A1, const Partition& B1,
		const Partition& A2, const Partition& B2
	) {
		
		
	}
#endif // _THREAD_

	_Mapptr_t _Mapptr;
	_NSTD uint _Mapsize, _Undersize;
};

#ifdef _NSTD_MATRIX_MULTITHREAD_













#endif // _THREAD_

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