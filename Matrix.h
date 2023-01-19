#pragma once
#ifndef _NSTD_MATRIX_
#define _NSTD_MATRIX_

#include <xmemory>
#include "Defines.h"
#include "TypeTraits.h"
#include "Threads.h"

_NSTD_BEGIN

// TODO vvv make comment vvv
#define _NSTD_MATRIX_MULTITHREAD_

//#ifdef _NSTD_MATRIX_MULTITHREAD_

//#endif

template <typename _Ty, typename _Alloc = _STD allocator<_Ty>>
class Matrix {
	using _Alty			= _STD _Rebind_alloc_t<_Alloc, _Ty>;
	using _Alty_traits	= _STD allocator_traits<_Alty>;
	using _Alpty		= _STD _Rebind_alloc_t<_Alloc, typename _Alty_traits::pointer>;
	using _Alpty_traits = _STD allocator_traits<_Alpty>;
	using _Mapptr_t		= typename _Alpty_traits::pointer;

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
	Matrix(_Mapptr_t init, const uint& row, const uint& col) : _Mapsize(row), _Undersize(col) {
		_Gen_map();
		_Set(init);
	}
	Matrix(const uint& row, const uint& col) : _Mapsize(row), _Undersize(col) {
		_Gen_map();
	}
	Matrix(const Matrix& other) : _Mapsize(other._Mapsize), _Undersize(other._Undersize) {
		_Gen_map();
		_Set(other._Mapptr);
	}
	~Matrix() { _Dealloc(); }



	_Ty& get(const uint& row, const uint& col) {
		return _Mapptr[row][col];
	}
	const _Ty& get(const uint& row, const uint& col) const {
		return _Mapptr[row][col];
	}

	Matrix& operator= (const Matrix& other) {
		_Resize_map(other._Mapsize, other._Undersize);
		_Set(other._Mapptr);
		return *this;
	}

private:
	// Assumes _Init has dimensions _Mapsize x _Undersize
	void _Set(_Mapptr_t _Init) {
		_NSTD_FOR_I(_Mapsize)
			_NSTD_FOR_J(_Undersize)
				_Mapptr[_I][_J] = _Init[_I][_J];
	}

	// Forgets old; Retains position if sizes are similar (for partitioning)
	void _Resize_map(uint _Newmapsize, uint _Newundersize) {
		if(_Mapsize != _Newmapsize || _Undersize != _Newundersize) {
			_Dealloc();
			_Mapsize = _Newmapsize;
			_Undersize = _Newundersize;
			_Gen_map();
		}
	}

	void _Gen_map() {
		_Alpty palloc;
		_Mapptr = _Alpty::allocate(palloc, _Mapsize);

		_Alty alloc;
		_NSTD_FOR_I(_Mapsize)
			_Mapptr[_I] = _Alty::allocate(alloc, _Undersize);
	}

	void _Dealloc() {
		_Dealloc_under();
		_Dealloc_map();
	}
	void _Dealloc_under() {
		_Alty alloc;
		_NSTD_FOR_I(_Mapsize) {
			_Alty::deallocate(alloc, _Mapptr[_I], _Undersize);
			_Mapptr[_I] = nullptr;
		}
	}
	void _Dealloc_map() {
		_Alpty palloc;
		_Alpty::deallocate(palloc, _Mapptr, _Mapsize);
		_Mapptr = nullptr;
	}

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
		_STD mutex _Add_mutex;
		
	}
#endif // _THREAD_

	_Mapptr_t _Mapptr;
	_NSTD uint _Mapsize, _Undersize;
};

#ifdef _NSTD_MATRIX_MULTITHREAD_

class _Matrix_threadpool {

};











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