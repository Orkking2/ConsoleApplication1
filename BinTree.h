#pragma once
#ifndef _NSTD_BINTREE_
#define _NSTD_BINTREE_

#include <algorithm>
#include <compare>
#include <concepts>
#include "Defines.h"
#include "TypeTraits.h"

_NSTD_BEGIN

template <typename T>
struct _Default_comparitor_t {
	static const _STD strong_ordering compare(const T& first, const T& second) {
		return first <=> second;
	}
};

template <typename _Val, typename _Key, typename _Comparitor>
struct _Default_node_t {
	// Will make a copy of _Val input
	_Default_node_t(const _Key& k, const _Val& v, _Default_node_t* l, _Default_node_t* r) : _key(k), _val(v), _left(l), _right(r) {}

	~_Default_node_t() { delete _left, _right; }

	_NODISCARD const _Val& GetVal(const _Key& key) const {
		switch (_Comparitor::compare(key, _key)) {
		case _STD strong_ordering::greater:
			_NSTD_ASSERT(_right != NULL && _right != nullptr, "_right is null");
			return _right->GetVal(key);
		case _STD strong_ordering::less:
			_NSTD_ASSERT(_left != NULL && _left != nullptr, "_left is null");
			return _left->GetVal(key);
		default:
			return _val;
		}
	}

	_Key _key;
	_Val _val;
	_Default_node_t* _left, * _right;
};

template <typename T, typename _Key, typename _Val>
concept _Good_array = requires(T t) {
    { t.size() }													-> _STD convertible_to<_NSTD uint>;
	{ t.begin() }													-> _STD same_as<auto>;
	{ t.end() }														-> _STD same_as<auto>;
	{ t[_STD declval<_NSTD add_cr_t<_NSTD uint>>()] }				-> _STD same_as<auto>;
	{ t[_STD declval<_NSTD add_cr_t<_NSTD uint>>()].get_first() }	-> _STD convertible_to<_Key>;
	{ t[_STD declval<_NSTD add_cr_t<_NSTD uint>>()].get_second() }	-> _STD convertible_to<_Val>;
};

template <typename _Key, typename _Val = void*, template <typename...> typename _Comparitor = _Default_comparitor_t, template <typename...> typename _Node = _Default_node_t>
class BinTree {
	using _Mykey_t		= _STD remove_cvref_t<_Key>;
	using _Myval_t		= _STD remove_cvref_t<_Val>;
	using _Mycompare_t	= _NSTD rebind_t<_Comparitor, _Mykey_t>;
	using _Mynode_t		= _NSTD rebind_t<_Node, _Mykey_t, _Myval_t, _Mycompare_t>;

	static_assert(_STD is_same_v<decltype(&_Mycompare_t::compare), _STD strong_ordering (_Mycompare_t::*)(_NSTD add_cr_t<_Mykey_t>, _NSTD add_cr_t<_Mykey_t>)>,
		"_Comparitor must have static method _STD strong_ordering compare(const _Key&, const _Key&) -- see _Default_comparitor_t");

	static_assert(_STD is_constructible_v<_Mynode_t, _NSTD add_cr_t<_Mykey_t>, _NSTD add_cr_t<_Myval_t>, _STD add_pointer_t<_Mynode_t>, _STD add_pointer_t<_Mynode_t>>,
		"_Node must have constructor _Node(const _Val&, const _Key&, _Node*, _Node*) -- see _Default_node_t");

	static_assert(_STD is_same_v<_NSTD add_cr_t<_Val>, decltype(_STD declval<_STD add_const_t<_Mynode_t>>().GetVal(_STD declval<_NSTD add_cr_t<_Mykey_t>>()))>,
		"_Node must have nonstatic method const _Val& GetVal(const _Key&) const -- see _Default_node_t");



public:
	using key_type			= _Mykey_t;
	using value_type		= _Myval_t;
	using comparitor_type	= _Mycompare_t;
	using node_type			= _Mynode_t;

	template <typename array_type>
		requires _Good_array<array_type, _Mykey_t, _Myval_t>
	BinTree(const array_type& arr) {
		_Head_node = genNode(arr);
	}

	~BinTree() { delete _Head_node; }

	template <typename array_type>
		requires _Good_array<array_type, _Mykey_t, _Myval_t>
	_NODISCARD _Mynode_t* genNode(array_type arr) const {
		_STD sort(arr.begin(), arr.end(), [](_Mykey_t a, _Mykey_t b) -> bool { return _Mycompare_t::compare(a, b) < 0; });
		return _Gen_node_unchecked(_Slice_t{ arr, 0, arr.length() });
	}

	_NODISCARD const _Myval_t& GetVal(const _Mykey_t& key) const {
		return _Head_node->GetVal(key);
	}

private:

	
	
	template <typename T>
	struct _Slice_t {
		_Slice_t(const _Slice_t& other, _NSTD uint _Off, _NSTD uint _Size) 
			: _Under(other._Under), _Off(_Off + other._Off), _Size(_Size) {}

		T& _Under;
		_NSTD uint _Off;
		_NSTD uint _Size;

		decltype(auto) begin() { return _Under.size() + _Off; }
		_NSTD uint length() { return _Size; }
	};

	template <typename T>
	_NODISCARD _Mynode_t* _Gen_node_unchecked(_Slice_t<T>&& _Slice) const {
		switch (_Slice.length()) {
		case 1:	return new _Mynode_t((*_Slice.begin()).get_second(), (*_Slice.begin()).get_first(), nullptr, nullptr);
		case 2: return new _Mynode_t((*_Slice.begin()).get_second(), (*_Slice.begin()).get_first(), nullptr, _Gen_node_unchecked(_Slice_t(_Slice, 1, 1)));
		default:
			return new _Mynode_t(
				(*(_Slice.begin() + _Slice.length() / 2)).get_second(), 
				(*(_Slice.begin() + _Slice.length() / 2)).get_first(),
				_Gen_node_unchecked(_Slice_t(_Slice, 0, _Slice.length() / 2)),
				_Gen_node_unchecked(_Slice_t(_Slice, _Slice.length() / 2 + 1, _Slice.length() - _Slice.length() / 2 - 1)));
		}
	}


	_Mynode_t* _Head_node;
};


_NSTD_END

#endif // _NSTD_BINTREE_