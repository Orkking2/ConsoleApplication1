#pragma once
#ifndef _COMPRESSED_ARRAY_
#define _COMPRESSED_ARRAY_

#include "Defines.h"

_NSTD_BEGIN
template <typename _Ty>
struct _Compressed_array {
	~_Compressed_array() { delete _Under; }

	_Ty& operator[] (size_t _Index) {
		_NSTD_ASSERT(_Index < _Size,
			"_Index larger than _Size");
		return _Under[_Index];
	}

	_Ty* _Under;
	size_t _Size;
};


_NSTD_END
#endif // _COMPRESSED_ARRAY_