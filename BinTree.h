#pragma once
#ifndef _NSTD_BINTREE_
#define _NSTD_BINTREE_

#include <utility>
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
	_Default_node_t(const _Val& v, const _Key& k, _Default_node_t* l, _Default_node_t* r) : _val(v), _key(k), _left(l), _right(r) {}

	_NODISCARD const _Val& GetVal(const _Key& key) const {
		switch (_Comparitor::compare(key, _key)) {
		case _STD strong_ordering::greater:
			_NSTD_ASSERT(_right != NULL && _right != nullptr, "_right is null");
			return _right->GetVal(key);
		case _STD strong::ordering:less:
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

template <typename _Key, typename _Val = void*, typename _Comparitor = _Default_comparitor_t<_Key>, typename _Node = _Default_node_t<_Key, _Val, _Comparitor>>
class BinTree {
	using _Mykey_t		= _NSTD remove_crv_t<_Key>;
	using _Myval_t		= _NSTD remove_crv_t<_Val>;
	using _Mycompare_t	= _NSTD rebind_t<_Comparitor, _Mykey_t>;
	using _Mynode_t		= _NSTD rebind_t<_Node, _Mykey_t, _Myval_t, _Mycompare_t>;

	static_assert(_STD is_same_v<decltype(&_Mycompare_t::compare), _STD strong_ordering (_Mycompare_t::*)(_NSTD add_cr_t<_Mykey_t>, _NSTD add_cr_t<_Mykey_t>)>,
		"_Comparitor must have static method _STD strong_ordering compare(const _Key&, const _Key&) -- see _Default_comparitor_t");

	static_assert(_STD is_constructible_v<_Mynode_t, _NSTD add_cr_t<_Myval_t>, _NSTD add_cr_t<_Mykey_t>, _STD add_pointer_t<_Mynode_t>, _STD add_pointer_t<_Mynode_t>>,
		"_Node must have constructor _Node(const _Val&, const _Key&, _Node*, _Node*) -- see _Default_node_t");

	static_assert(_STD is_same_v<_NSTD add_cr_t<_Val>, decltype(_STD declval<_STD add_const_t<_Mynode_t>>().compare(_STD declval<_NSTD add_cr_t<_Mykey_t>>()))>,
		"_Node must have nonstatic method const _Val& GetVal(const _Key&) const -- see _Default_node_t");



public:
	using key_type			= _Mykey_t;
	using value_type		= _Myval_t;
	using comparitor_type	= _Mycompare_t;
	using node_type			= _Mynode_t;

	template <typename size_type, typename array_type, typename = _STD enable_if_t<_NSTD is_indexable_by<_NSTD add_cr_t<array_type>, _NSTD add_cr_t<size_type>, _Mykey_t>()>>
	BinTree(const array_type& arr, const size_type& size) {

	}

	_NODISCARD const _Myval_t& GetVal(const _Mykey_t& key) {
		return _Head_node.GetVal(key);
	}

private:
	_Mynode_t* _Head_node;
};


_NSTD_END

#endif // _NSTD_BINTREE_