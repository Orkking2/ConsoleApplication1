#pragma once
#ifndef _NSTD_HUFF_
#define _NSTD_HUFF_

#include <unordered_map>
#include <vector>
#include <deque>
#include "Defines.h"

_NSTD_BEGIN

class HuffTree {
	typedef unsigned int uint;

public:
	struct IElement {
		virtual ~IElement() = default;
		virtual const void* GetP(_STD deque<bool>&)  const = 0;
		virtual const uint GetFreq()                 const = 0;
		virtual const bool isNode()                  const = 0;
	};

	class vPtr : public IElement {
		void* ptr_;
		uint freq_;
	public:
		vPtr(void* ptr = NULL, const uint freq = 0) : ptr_(ptr), freq_(freq) {}

		const void* GetP(_STD deque<bool>&) const override { return ptr_; }
		const uint GetFreq()                const override { return freq_; }
		const bool isNode()                 const override { return false; }
	};

	class Node : public IElement {
		const IElement* l_, * r_;
	public:
		Node(const IElement* l, const IElement* r) : l_(l), r_(r) {}
		const void* GetP(_STD deque<bool>&) const override;
		const uint GetFreq()                const override { return (l_->GetFreq() + r_->GetFreq()); }
		const bool isNode()                 const override { return true; }
	};
private:
	_STD vector<IElement*> Nodes_;
	_STD unordered_map<void*, _STD vector<bool>> map_;
public:
	HuffTree() : Nodes_(), map_() {}
	template <class _Ty>
	HuffTree(_STD vector<_Ty> objs) {
		Nodes_.resize(objs.size(), NULL);
		for (_Ty& obj : objs) {

		}
	}
};

_NSTD_END
#endif