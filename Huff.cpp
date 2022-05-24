#include "Huff.h"
#ifdef _NSTD_HUFF_

_NSTD_BEGIN

const void*& HuffTree::Node::GetP(_STD deque<bool>& deque) const {
	bool cashe = deque.front();
	deque.pop_front();
	switch (cashe) {
	case false: return l_->GetP(deque);
	case true:  return r_->GetP(deque);
	}
}




_NSTD_END
#else // ^^^^ _NSTD_HUFF_ / !_NSTD_HUFF_ vvvv
#pragma once(message, "Huff.h not found")
#endif 