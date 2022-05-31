#pragma once
#ifndef _NSTD_HUFF_
#define _NSTD_HUFF_

#include <unordered_map>
#include <assert.h>
#include <vector>
#include <deque>
#include "Defines.h"

_NSTD_BEGIN

class HuffTree {
	typedef unsigned int uint;
	typedef _STD pair<void*, int> vfp;

public:
	struct IElement {
		virtual ~IElement() = default;
		virtual const void* GetP(_STD deque<bool>&) const = 0;
		virtual const bool isNode()                 const = 0;
		virtual const uint GetFreq()                const = 0;
		virtual _STD vector<void*> vpArr()          const = 0;
	};

	class vPtr : public IElement {
		bool is_set_;

		void* ptr_;
		uint freq_;
	public:
		vPtr(void* ptr, const uint freq) : ptr_(ptr), freq_(freq), is_set_(true) {}
		vPtr(_STD pair<void*, uint> val_pair) : ptr_(val_pair.first), freq_(val_pair.second), is_set_(true) {}

		_NODISCARD const void* GetP(_STD deque<bool>&) const override { return ptr_; }
		_NODISCARD const bool isNode()                 const override { return false; }
		_NODISCARD const uint GetFreq()                const override { return freq_; }
		_NODISCARD _STD vector<void*> vpArr()          const override { return { ptr_ }; }
	};

	class Node : public IElement {
		bool is_set_;

		IElement* l_, * r_;
	public:
		Node(IElement* l, IElement* r) : l_(l), r_(r), is_set_(true) {}
		~Node() override {
			if (l_->isNode())
				delete reinterpret_cast<Node*> (l_);
			else
				delete reinterpret_cast<vPtr*> (l_);
			if (r_->isNode())
				delete reinterpret_cast<Node*> (r_);
			else
				delete reinterpret_cast<vPtr*> (r_);
		}

		_NODISCARD const void* GetP(_STD deque<bool>&) const override;
		_NODISCARD const bool isNode()                 const override { return true; }
		_NODISCARD const uint GetFreq()                const override { return l_->GetFreq() + r_->GetFreq(); }
		_NODISCARD _STD vector<void*> vpArr()          const override;
	};
private:
	bool ptrs_set_;
	_STD vector<void*> ptrs_;
	_STD unordered_map<void*, _STD vector<bool>> map_;

	IElement* Fin_node_;
public:
	HuffTree() : ptrs_set_(false), Fin_node_(NULL) {}
	~HuffTree() {
		assert(!ptrs_set_);
	}

	template <class _Ty>
	void release_ptrs() {
		for (void*& ptr : ptrs_) 
			delete reinterpret_cast<_Ty*> (ptr);
		release_ptrs_NODELETE();
	}

	void release_ptrs_NODELETE();

	void create_tree(_STD vector<_STD pair<void*, uint>> vals);
	void create_tree_NOREVERSE(_STD vector<_STD pair<void*, uint>> vals);

	template <class _Ty>
	_NODISCARD static _STD vector<_STD pair<void*, uint>> gen_freqs(_STD vector<_Ty> vals) {
		_STD unordered_map<_Ty, uint> checker;
		_STD vector<void*> unique_ptrs;
		for (_Ty& val : vals) {
			if (!checker.contains(val)) {
				// If a HuffTree object creates a tree using this method, 
				// it should call HuffTree::release_ptrs<_Ty>()
				unique_ptrs.push_back(reinterpret_cast<void*> (new _Ty(val)));
				checker[val] = 1;
			} else {
				checker[val]++;
			}
		}

		_STD vector<_STD pair<void*, uint>> out;
		for (void*& p : unique_ptrs) 
			out.push_back(_STD pair<void*, uint>(p, *reinterpret_cast<_Ty*> (p)));
		
		return out;
	}

	template <class _Ty>
	_NODISCARD _STD unordered_map<_Ty, _STD vector<bool>> gen_val_map() {
		_STD unordered_map<_Ty, _STD vector<bool>> out;
		if (ptrs_set_) {
			for (void* p : ptrs_) {
				const _Ty* pVal = reinterpret_cast<_Ty*> (p);
				out[*pVal] = map_[p];
			}
		}
		return out;
	}

	template <class _Ty>
	_NODISCARD _STD vector<_Ty> decode(_STD deque<bool>& bool_list) {
		_STD vector<_Ty> out;
		if (ptrs_set_) {
			while (!bool_list.empty())
				out.push_back(*reinterpret_cast<_Ty*> (const_cast<void*> (Fin_node_->GetP(bool_list)));
		}
		return out;
	}

	template <>
	_NODISCARD _STD vector<void*> decode<void*>(_STD deque<bool>& bool_list) {
		_STD vector<void*> out;
		if (ptrs_set_) {
			while (!bool_list.empty())
				out.push_back(const_cast<void*> (Fin_node_->GetP(bool_list)));
		}
	}
};

_NSTD_END
#endif