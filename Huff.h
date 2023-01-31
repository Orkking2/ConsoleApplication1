#pragma once
#ifndef _NSTD_HUFF_
#define _NSTD_HUFF_

#include <unordered_map>
#include <vector>
#include "Deque.h"
#include "Defines.h"
#include "TypeTraits.h"

_NSTD_BEGIN

template <class _Iter>
concept __huffman_good_iter = _STD convertible_to<_Iter, bool> &&
	_STD is_same_v<_Iter&, decltype(_STD declval<_Iter>().operator++(0))> &&
	_STD convertible_to<const _Iter&, _Iter>;

template <typename _Ty>
class HuffTree {
	template <typename T> using __unref = _STD remove_cvref_t<T>;
	using __unref_ty = __unref<_Ty>;

	class __any_iterator_interface {
	public:
		virtual ~__any_iterator_interface() = default;
		virtual __any_iterator_interface* clone() const = 0;
		virtual __any_iterator_interface* operator++(int) = 0;
		virtual operator bool() const = 0;
	};

	template <class _Iter>
		requires __huffman_good_iter<_Iter>
	class __any_iterator_holder : public __any_iterator_interface {
	public:
		__any_iterator_holder(const _Iter& base) : _base(base) {}
		__any_iterator_interface* clone()	const override { return new __any_iterator_holder(  _base); }
		__any_iterator_interface* operator++(int) override { return new __any_iterator_holder(++_base); }
		operator bool()						const override { return static_cast<bool>(_base); }
	private:
		_Iter _base;
	};

	// Must always wrap an iterator
	class __any_iterator {
	public:
		template <class _Iter>
		__any_iterator(const _Iter& iter)			: _contents(new __any_iterator_holder<_Iter>(iter)) {}
		__any_iterator(const __any_iterator& other) : _contents(other._contents->clone())				{}

		__any_iterator(__any_iterator_interface* contents) : _contents(contents) {
			_NSTD_ASSERT(_contents != NULL && _contents != nullptr,
				"__any_iterator must always wrap an iterator");
		}

		~__any_iterator() { delete _contents; }

		__any_iterator operator++(int) { return __any_iterator(_contents->operator++(0)); }
		operator bool() { return _contents->operator bool(); }

	private:
		__any_iterator_interface* _contents;
	};



	struct __element_interface {
		virtual ~__element_interface() = default;
		virtual       __unref_ty& _get(__any_iterator&)       = 0;
		virtual const __unref_ty& _get(__any_iterator&) const = 0;
		//virtual const bool _is_node() const = 0;
		virtual const uint _getFreq() const = 0;
		//virtual _STD vector<_Ty&> _arr() const = 0;
	};

	// Can own or hold reference to _ref (_Ty can be reference type)
	class __element : public __element_interface {
	public:
		__element(_Ty ref) : _ref(ref) {}
		      __unref_ty& _get(__any_iterator&)       override { return  _ref; }
		const __unref_ty& _get(__any_iterator&) const override { return  _ref; }
		const uint		  _getFreq()			const override { return _freq; }

	private:
		_Ty _ref;
		uint _freq;
	};

	
	class __element_node : public __element_interface {
	public:
		__element_node(__element_interface* left, __element_interface* right) : _left(left), _right(right) {
			_NSTD_ASSERT(_left != NULL && _left != nullptr && _right != NULL && _right != nullptr,
				"__element_node may not contain invalid _left or _right pointers");
		};
		const uint _getFreq() const override { 
			return _left->_getFreq() + _right->_getFreq(); 
		}
		__unref_ty& _get(__any_iterator& _iter) override {
			if(++_iter)
				return _right->_get(_iter);
			return _left->_get(_iter);
		}
		const __unref_ty& _get(__any_iterator& _iter) const override {
			if(++_iter)
				return _right->_get(_iter);
			return _left->_get(_iter);
		}
		~__element_node() override { delete _left, _right; }
	private:
		__element_interface* _left, * _right;
	};








};



//class HuffTree {
//	typedef _NSTD uint uint;
//	typedef _STD pair<void*, uint> _Pair_vpu;
//
//public:
//	struct IElement {
//		virtual ~IElement() = default;
//		virtual const void* GetP(_NSTD deque<bool>&) const = 0;
//		virtual const bool isNode()                  const = 0;
//		virtual const uint GetFreq()                 const = 0;
//		virtual _STD vector<void*> vpArr()           const = 0;
//	};
//
//	class vPtr : public IElement {
//		void* ptr_;
//		uint freq_;
//	public:
//		vPtr(void* ptr, const uint freq) : ptr_(ptr),            freq_(freq)            {}
//		vPtr(_Pair_vpu val_pair)         : ptr_(val_pair.first), freq_(val_pair.second) {}
//
//		_NODISCARD const void* GetP(_NSTD deque<bool>&) const override { return ptr_; }
//		_NODISCARD const bool isNode()                  const override { return false; }
//		_NODISCARD const uint GetFreq()                 const override { return freq_; }
//		_NODISCARD _STD vector<void*> vpArr()           const override { return { ptr_ }; }
//	};
//
//	class Node : public IElement {
//		IElement* l_, * r_;
//	public:
//		Node(IElement* l, IElement* r) : l_(l), r_(r){}
//		~Node() override { delete l_, r_; }
//
//		_NODISCARD const void* GetP(_NSTD deque<bool>& deque) const override { return deque.pop_front() ? r_->GetP(deque) : l_->GetP(deque); }
//		_NODISCARD const bool isNode()                        const override { return true; }
//		_NODISCARD const uint GetFreq()                       const override { return l_->GetFreq() + r_->GetFreq(); }
//		_NODISCARD _STD vector<void*> vpArr()                 const override;
//	};
//private:
//	bool ptrs_set_;
//	_STD vector<void*> ptrs_;
//	_STD unordered_map<void*, _STD vector<bool>> map_;
//
//	IElement* Fin_node_;
//public:
//	HuffTree()                            : ptrs_set_(false), Fin_node_(NULL) {}
//	HuffTree(_STD vector<_Pair_vpu> vals) : ptrs_set_(false), Fin_node_(NULL) { create_tree(vals); }
//	~HuffTree() {
//		release_ptrs();
//	}
//
//	// Debugging
//	_STD vector<void*>& get_ptrs() {
//		return ptrs_;
//	}
//
//	void release_ptrs() {
//		for (void*& ptr : ptrs_) 
//			delete ptr;
//		release_ptrs_NODELETE();
//	}
//
//	void release_ptrs_NODELETE();
//
//	void create_tree(_STD vector<_Pair_vpu> vals);
//	void create_tree_NOREVERSE(_STD vector<_Pair_vpu> vals);
//
//	template <class _Ty>
//	_NODISCARD static _STD vector<_Pair_vpu> gen_freqs(_STD vector<_Ty> vals) {
//		_STD unordered_map<_Ty, uint> checker;
//		_STD vector<void*> unique_ptrs;
//		for (const _Ty& val : vals) {
//			if (!checker.contains(val)) {
//				// If a HuffTree object creates a tree using this method, 
//				// it should call HuffTree::release_ptrs<_Ty>()
//				unique_ptrs.push_back(reinterpret_cast<void*> (new _Ty(val)));
//				checker[val] = 1;
//			} else {
//				checker[val]++;
//			}
//		}
//
//		_STD vector<_Pair_vpu> out;
//		for (void*& p : unique_ptrs) 
//			out.push_back(_Pair_vpu(p, checker[*reinterpret_cast<_Ty*> (p)]));
//		
//		return out;
//	}
//
//	template <class _Ty>
//	_NODISCARD _STD unordered_map<_Ty, _STD vector<bool>> gen_val_map() {
//		_STD unordered_map<_Ty, _STD vector<bool>> out;
//		if (ptrs_set_) {
//			for (void*& p : ptrs_) {
//				const _Ty* pVal = reinterpret_cast<_Ty*> (p);
//				out[*pVal] = map_[p];
//			}
//		}
//		return out;
//	}
//
//	template <class _Ty>
//	_NODISCARD _NSTD deque<bool> encode(_STD vector<_Ty> vals) {
//		_STD unordered_map<_Ty, _STD vector<bool>> map(gen_val_map<_Ty>());
//		_NSTD deque<bool> out;
//		if (ptrs_set_) {
//			for (const _Ty& val : vals) {
//				if (map.contains(val)) {
//					for (bool b : map[val]) {
//						out.push_back(b);
//					}
//				}
//			}
//		}
//		return out;
//	}
//
//	template <>
//	// Heterogenous encode
//	_NODISCARD _NSTD deque<bool> encode<void*>(_STD vector<void*> ptrs) {
//		_NSTD deque<bool> out;
//		if (ptrs_set_) {
//			for (void*& p : ptrs) {
//				for (const bool& b : map_[p]) {
//					out.push_back(b);
//				}
//			}
//		}
//		return out;
//	}
//
//	template <class _Ty>
//	_NODISCARD _STD vector<_Ty> decode(_NSTD deque<bool> bool_list) {
//		_STD vector<_Ty> out;
//		if (ptrs_set_) {
//			while (!bool_list.empty())
//				out.push_back(*reinterpret_cast<_Ty*> (const_cast<void*> (Fin_node_->GetP(bool_list))));
//		}
//		return out;
//	}
//
//	template <>
//	// Heterogenous decode
//	_NODISCARD _STD vector<void*> decode<void*>(_NSTD deque<bool> bool_list) {
//		_STD vector<void*> out;
//		if (ptrs_set_) {
//			while (!bool_list.empty())
//				out.push_back(const_cast<void*> (Fin_node_->GetP(bool_list)));
//		}
//		return out;
//	}
//};
//
_NSTD_END


//#endif // !_NSTD_HUFF_
//
//#ifdef _NSTD_HUFF_DEBUGGING_
//#include <iostream>
//#include <functional>
//template <class _Ty>
//void test(nstd::uint num_tests, _STD function<_Ty(void)> rand_generator, bool err_switch = false) {
//	std::vector<_Ty> vals;
//	vals.resize(num_tests);
//	_NSTD_FOR_I(num_tests)
//		vals[_I] = rand_generator();
//
//	nstd::HuffTree t1(nstd::HuffTree::gen_freqs<_Ty>(vals));
//	_NSTD deque<bool> d(t1.encode(vals));
//	if (err_switch) {
//		unsigned int error_pos(rand() % d.size());
//		// Introduce error
//		d.flip(error_pos);
//		std::cout << "Error at pos " << error_pos << '\n';
//	}
//	int d_sz_cashe(d.real_size());
//	_STD vector<_Ty> v(t1.decode<_Ty>(d.shrink_fit()));
//
//	if (vals.size() != v.size()) {
//		std::cout << "ERROR: vals.size() = " << vals.size() << " != v.size() = " << v.size();
//		return;
//	}
//
//	_STD vector<unsigned int> indexi;
//	_NSTD_FOR_I(vals.size()) {
//		if (vals[_I] != v[_I]) {
//			indexi.push_back(_I);
//		}
//	}
//	std::cout << "Errors: ";
//	if (!indexi.size())
//		std::cout << "None";
//	_NSTD_FOR_I(indexi.size())
//		std::cout << indexi[_I] << ", ";
//	if (!indexi.empty())
//		std::cout << "\nFrom ____ to ____:\n";
//	_NSTD_FOR_I(indexi.size())
//		std::cout << vals[indexi[_I]] << "-" << v[indexi[_I]] << ", ";
//
//
//	std::cout << "\nSize of original array:\n" << sizeof(_Ty) * vals.size();
//	std::cout << "\nSize unshrunk:\n" << d_sz_cashe;
//	std::cout << "\nUnshrunk percent compression:\n" << (1 - static_cast<double> (d_sz_cashe) / (sizeof(_Ty) * vals.size())) * 100 << '%';
//	std::cout << "\nSize shrunk:\n" << d.real_size();
//	std::cout << "\nShrunk percent compression:\n" << (1 - static_cast<double> (d.real_size()) / (sizeof(_Ty) * vals.size())) * 100 << '%';
//}
#endif // _NSTD_HUFF_DEBUGGING_