#pragma once

#include "Defines.h"
#include <functional>
#include <vector>

#ifndef _NSTD_SORT_
#define _NSTD_SORT_

_NSTD_BEGIN


_MULTITHREAD_BEGIN
template <class _Ty, class _List = _STD vector<_Ty>>
_List sort(_List l, _STD function<bool(_Ty, _Ty)> less_than = [](_Ty l, _Ty r) -> bool { return l < r; }) {

}








_MULTITHREAD_END
_NSTD_END
#endif // !_NSTD_SORT_