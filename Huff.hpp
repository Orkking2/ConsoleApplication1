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
	using __unref_t = __unref<_Ty>;

public:
	using value_type = __unref_t;


private:
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
		__any_iterator_interface* clone() const override { return new __any_iterator_holder(  _base); }
		__any_iterator_interface* operator++(int) override { return new __any_iterator_holder(++_base); }
		operator bool()	const override { return static_cast<bool>(_base); }
	private:
		_Iter _base;
	};

	// Must always wrap an iterator
	class __any_iterator {
	public:
		template <class _Iter>
		__any_iterator(const _Iter& iter) : _contents(new __any_iterator_holder<_Iter>(iter)) {}
		__any_iterator(const __any_iterator& other) : _contents(other._contents->clone()) {}

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
		virtual __unref_t& _get(__any_iterator&) = 0;
		virtual const __unref_t& _get(__any_iterator&) const = 0;
		//virtual const bool _is_node() const = 0;
		virtual const uint _getFreq() const = 0;
		virtual _STD vector<const __unref_t&> _genArr() const = 0;
		virtual __element_interface* clone() const = 0;
	};

	// Can own or hold reference to _ref (_Ty can be reference type)
	class __element : public __element_interface {
	public:
		__element(_Ty ref, uint freq) : _ref(ref), _freq(freq) {}

		__unref_t& _get(__any_iterator&) override { 
			return  _ref; 
		}
		const __unref_t& _get(__any_iterator&) const override { 
			return  _ref; 
		}
		const uint _getFreq() const override { 
			return _freq; 
		}
		_STD vector<const __unref_t&> _genArr() { 
			return { _ref }; 
		}
		__element_interface* clone() const override { 
			return new __element(_ref, _freq); 
		}

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
		__unref_t& _get(__any_iterator& _iter) override {
			if(++_iter)
				return _right->_get(_iter);
			return _left->_get(_iter);
		}
		const __unref_t& _get(__any_iterator& _iter) const override {
			if(++_iter)
				return _right->_get(_iter);
			return _left->_get(_iter);
		}
		_STD vector<const __unref_t&> _genArr() {
			_STD vector<const __unref_t&> out(_left->_genArr());
			_STD vector<const __unref_t&> temp(_right->_genArr());
			out.insert(out.end(), temp.begin(), temp.end());
			return out;
		}
		__element_interface* clone() const override {
			return new __element_node(_left->clone(), _right->clone());
		}
		~__element_node() override { delete _left, _right; }
	private:
		__element_interface* _left, * _right;
	};

public:
	template <template <typename...> class _Array, typename... _ATraits, template <typename...> class _Pair, typename... _PTraits>
	HuffTree(_Array<_Pair<_Ty, uint, _PTraits...>, _ATraits...> list) { gen_tree(list); }

	HuffTree(const HuffTree& other) : _root(other._root->clone()) {}
	HuffTree() : _root(nullptr) {}

	template <template <typename...> class _Array, typename... _ATraits, template <typename...> class _Pair, typename... _PTraits>
	void gen_tree(_Array<_Pair<_Ty, uint, _PTraits...>, _ATraits...> list) {
		this->~HuffTree();

		if(!list.size())
			return;
		if(list.size() == 1) {
			_root = new __element(list[0].first, list[0].second);
			return;
		}
		
		_STD vector<__element_interface*> _elements;
		_NSTD_FOR_I(list.size()) {
			if(!_encoder_map.contains(list[_I].first)) {
				_encoder_map.insert({ list[_I].first, _STD vector<bool>() });
				_elements.push_back(new __element(list[_I].first, list[_I].second));
			}	
		}
		
		while(_elements.size() != 1) {
			uint lindex = 0, hindex = 0;
			_NSTD_FOR_I(_elements.size()) {
				if(_elements[_I]->_getFreq() < _elements[lindex]->_getFreq()) {
					hindex = lindex;
					lindex = _I;
				} else if(hindex == lindex || _elements[_I]->_getFreq() < _elements[hindex]->_getFreq()) {
					hindex = _I;
				}
			}
			__element_interface* temp = new __element_node(_elements[lindex], _elements[hindex]);

			_STD vector<const __unref_t&> ltemp(_elements[lindex]->_genArr());
			for(const __unref_t& t : ltemp)
				_encoder_map.at(t).push_back(false);
			
			_STD vector<const __unref_t&> rtemp(_elements[hindex]->_genArr());
			for(const __unref_t& t : rtemp)
				_encoder_map.at(t).push_back(true);
			
			_elements.erase(_elements.begin() + lindex);
			_elements.erase(_elements.begin() + hindex);

			_elements.push_back(temp);
		}

		_root = _elements[0];
	}

	template <class _Arr>
	_STD vector<bool> encode(_Arr arr) {
		encode(arr.begin(), arr.end());
	}

	template <class _Iter>
	_STD vector<bool> encode(_Iter begin, _Iter end) {
		_NSTD_ASSERT(!empty(),
			"encode called with empty root");
		_STD vector<bool> out;
		for(_Iter i = begin; i != end; i++)
			out.insert(out.end(), _encoder_map.at(i).begin(), _encoder_map.at(i).end());
		return out;
	}

	template <class _Iter>
	__unref_t decode(_Iter iter) {
		return _root->_get(iter);
	}

	template <class _Iter>
	const __unref_t& decode(_Iter iter) const {
		return _root->_get(iter);
	}

	template <template <typename...> class _Arr, template <typename> class _Alloc, typename... _ExtraTraits>
	_Arr<__unref_t, _Alloc<__unref_t>, _ExtraTraits...> decode(_Arr<bool, _Alloc<bool>, _ExtraTraits...> arr) {
		_Arr<__unref_t, _Alloc<__unref_t>> out;
		for(auto i = arr.begin(); i != arr.end(); i++)
			out.push_back(decode(i));
		
		return out;
	}

	template <template <typename...> class _Arr, template <typename> class _Alloc, typename... _ExtraTraits>
	_Arr<const __unref_t&, _Alloc<const __unref_t&>, _ExtraTraits...> decode(_Arr<bool, _Alloc<bool>, _ExtraTraits...> arr) const {
		_Arr<const __unref_t&, _Alloc<const __unref_t&>> out;
		for(auto i = arr.begin(); i != arr.end(); i++)
			out.push_back(decode(i));

		return out;
	}

	const bool empty() {
		return _root == nullptr || _root == NULL;
	}

	~HuffTree() { delete _root; _root = nullptr; _encoder_map.clear(); }

private:


	__element_interface* _root;
	_STD unordered_map<__unref_t, _STD vector<bool>> _encoder_map;
};

_NSTD_END
#endif // !_NSTD_HUFF_