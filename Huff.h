#pragma once
#ifndef _NSTD_HUFF_
#define _NSTD_HUFF_

#include <unordered_map>
#include <vector>
#include "Deque.h"
#include "Defines.h"

_NSTD_BEGIN

class HuffTree {
	typedef _NSTD uint uint;
	typedef _STD pair<void*, uint> _Pair_vpu;

public:
	struct IElement {
		virtual ~IElement() = default;
		virtual const void* GetP(_NSTD deque<bool>&) const = 0;
		virtual const bool isNode()                  const = 0;
		virtual const uint GetFreq()                 const = 0;
		virtual _STD vector<void*> vpArr()           const = 0;
	};

	class vPtr : public IElement {
		void* ptr_;
		uint freq_;
	public:
		vPtr(void* ptr, const uint freq) : ptr_(ptr),            freq_(freq)            {}
		vPtr(_Pair_vpu val_pair)         : ptr_(val_pair.first), freq_(val_pair.second) {}

		_NODISCARD const void* GetP(_NSTD deque<bool>&) const override { return ptr_; }
		_NODISCARD const bool isNode()                  const override { return false; }
		_NODISCARD const uint GetFreq()                 const override { return freq_; }
		_NODISCARD _STD vector<void*> vpArr()           const override { return { ptr_ }; }
	};

	class Node : public IElement {
		IElement* l_, * r_;
	public:
		Node(IElement* l, IElement* r) : l_(l), r_(r){}
		~Node() override { delete l_, r_; }

		_NODISCARD const void* GetP(_NSTD deque<bool>& deque) const override { return deque.pop_front() ? r_->GetP(deque) : l_->GetP(deque); }
		_NODISCARD const bool isNode()                        const override { return true; }
		_NODISCARD const uint GetFreq()                       const override { return l_->GetFreq() + r_->GetFreq(); }
		_NODISCARD _STD vector<void*> vpArr()                 const override;
	};
private:
	bool ptrs_set_;
	_STD vector<void*> ptrs_;
	_STD unordered_map<void*, _STD vector<bool>> map_;

	IElement* Fin_node_;
public:
	HuffTree()                            : ptrs_set_(false), Fin_node_(NULL) {}
	HuffTree(_STD vector<_Pair_vpu> vals) : ptrs_set_(false), Fin_node_(NULL) { create_tree(vals); }
	~HuffTree() {
		release_ptrs();
	}

	// Debugging
	_STD vector<void*>& get_ptrs() {
		return ptrs_;
	}

	void release_ptrs() {
		for (void*& ptr : ptrs_) 
			delete ptr;
		release_ptrs_NODELETE();
	}

	void release_ptrs_NODELETE();

	void create_tree(_STD vector<_Pair_vpu> vals);
	void create_tree_NOREVERSE(_STD vector<_Pair_vpu> vals);

	template <class _Ty>
	_NODISCARD static _STD vector<_Pair_vpu> gen_freqs(_STD vector<_Ty> vals) {
		_STD unordered_map<_Ty, uint> checker;
		_STD vector<void*> unique_ptrs;
		for (const _Ty& val : vals) {
			if (!checker.contains(val)) {
				// If a HuffTree object creates a tree using this method, 
				// it should call HuffTree::release_ptrs<_Ty>()
				unique_ptrs.push_back(reinterpret_cast<void*> (new _Ty(val)));
				checker[val] = 1;
			} else {
				checker[val]++;
			}
		}

		_STD vector<_Pair_vpu> out;
		for (void*& p : unique_ptrs) 
			out.push_back(_Pair_vpu(p, checker[*reinterpret_cast<_Ty*> (p)]));
		
		return out;
	}

	template <class _Ty>
	_NODISCARD _STD unordered_map<_Ty, _STD vector<bool>> gen_val_map() {
		_STD unordered_map<_Ty, _STD vector<bool>> out;
		if (ptrs_set_) {
			for (void*& p : ptrs_) {
				const _Ty* pVal = reinterpret_cast<_Ty*> (p);
				out[*pVal] = map_[p];
			}
		}
		return out;
	}

	template <class _Ty>
	_NODISCARD _NSTD deque<bool> encode(_STD vector<_Ty> vals) {
		_STD unordered_map<_Ty, _STD vector<bool>> map(gen_val_map<_Ty>());
		_NSTD deque<bool> out;
		if (ptrs_set_) {
			for (const _Ty& val : vals) {
				if (map.contains(val)) {
					for (bool b : map[val]) {
						out.push_back(b);
					}
				}
			}
		}
		return out;
	}

	template <>
	// Heterogenous encode
	_NODISCARD _NSTD deque<bool> encode<void*>(_STD vector<void*> ptrs) {
		_NSTD deque<bool> out;
		if (ptrs_set_) {
			for (void*& p : ptrs) {
				for (const bool& b : map_[p]) {
					out.push_back(b);
				}
			}
		}
		return out;
	}

	template <class _Ty>
	_NODISCARD _STD vector<_Ty> decode(_NSTD deque<bool> bool_list) {
		_STD vector<_Ty> out;
		if (ptrs_set_) {
			while (!bool_list.empty())
				out.push_back(*reinterpret_cast<_Ty*> (const_cast<void*> (Fin_node_->GetP(bool_list))));
		}
		return out;
	}

	template <>
	// Heterogenous decode
	_NODISCARD _STD vector<void*> decode<void*>(_NSTD deque<bool> bool_list) {
		_STD vector<void*> out;
		if (ptrs_set_) {
			while (!bool_list.empty())
				out.push_back(const_cast<void*> (Fin_node_->GetP(bool_list)));
		}
		return out;
	}
};

_NSTD_END
#endif // !_NSTD_HUFF_

#ifdef _NSTD_HUFF_DEBUGGING_
#include <iostream>
#include <functional>
template <class _Ty>
void test(nstd::uint num_tests, _STD function<_Ty(void)> rand_generator, bool err_switch = false) {
	std::vector<_Ty> vals;
	vals.resize(num_tests);
	_NSTD_FOR_I(num_tests)
		vals[_I] = rand_generator();

	nstd::HuffTree t1(nstd::HuffTree::gen_freqs<_Ty>(vals));
	_NSTD deque<bool> d(t1.encode(vals));
	if (err_switch) {
		unsigned int error_pos(rand() % d.size());
		// Introduce error
		d.flip(error_pos);
		std::cout << "Error at pos " << error_pos << '\n';
	}
	int d_sz_cashe(d.real_size());
	_STD vector<_Ty> v(t1.decode<_Ty>(d.shrink_fit()));

	if (vals.size() != v.size()) {
		std::cout << "ERROR: vals.size() = " << vals.size() << " != v.size() = " << v.size();
		return;
	}

	_STD vector<unsigned int> indexi;
	_NSTD_FOR_I(vals.size()) {
		if (vals[_I] != v[_I]) {
			indexi.push_back(_I);
		}
	}
	std::cout << "Errors: ";
	if (!indexi.size())
		std::cout << "None";
	_NSTD_FOR_I(indexi.size())
		std::cout << indexi[_I] << ", ";
	if (!indexi.empty())
		std::cout << "\nFrom ____ to ____:\n";
	_NSTD_FOR_I(indexi.size())
		std::cout << vals[indexi[_I]] << "-" << v[indexi[_I]] << ", ";


	std::cout << "\nSize of original array:\n" << sizeof(_Ty) * vals.size();
	std::cout << "\nSize unshrunk:\n" << d_sz_cashe;
	std::cout << "\nUnshrunk percent compression:\n" << (1 - static_cast<double> (d_sz_cashe) / (sizeof(_Ty) * vals.size())) * 100 << '%';
	std::cout << "\nSize shrunk:\n" << d.real_size();
	std::cout << "\nShrunk percent compression:\n" << (1 - static_cast<double> (d.real_size()) / (sizeof(_Ty) * vals.size())) * 100 << '%';
}
#endif // _NSTD_HUFF_DEBUGGING_