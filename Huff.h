#pragma once
#ifndef _NSTD_HUFF_
#define _NSTD_HUFF_

#include <vector>
#include <deque>
#include <unordered_map>
#include <memory>
#include "Defines.h"

_NSTD_BEGIN

class HuffTree {

};

struct IElement {
	~IElement() = default;
	virtual const void*& GetP(_STD deque<bool>&) = 0;
	virtual const unsigned int& GetFreq() = 0;
	virtual const bool& isNode() = 0;
};

class vPtr : IElement {
	const void* ptr_;
	const int freq_;
public:
	vPtr(const void* ptr = NULL, const int freq = 0) : ptr_(ptr), freq_(freq) {}


};

_NSTD_END
#endif