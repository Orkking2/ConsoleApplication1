#pragma once
#ifndef _NSTD_BINTREE_
#define _NSTD_BINTREE_

#include <utility>
#include <memory>
#include <compare>
#include <concepts>
#include <algorithm>
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

template <typename _Key, typename _Val = void*, typename _Comparitor = _Default_comparitor_t<_Key>, typename _Node = _Default_node_t<_Key, _Val, _Comparitor>>
class BinTree {
	using _Mykey_t		= _STD remove_cvref_t<_Key>;
	using _Myval_t		= _STD remove_cvref_t<_Val>;
	using _Mycompare_t	= _NSTD rebind_t<_Comparitor, _Mykey_t>;
	using _Mynode_t		= _NSTD rebind_t<_Node, _Mykey_t, _Myval_t, _Mycompare_t>;

	static_assert(_STD is_same_v<decltype(&_Mycompare_t::compare), _STD strong_ordering (_Mycompare_t::*)(_NSTD add_cr_t<_Mykey_t>, _NSTD add_cr_t<_Mykey_t>)>,
		"_Comparitor must have static method _STD strong_ordering compare(const _Key&, const _Key&) -- see _Default_comparitor_t");

	static_assert(_STD is_constructible_v<_Mynode_t, _NSTD add_cr_t<_Myval_t>, _NSTD add_cr_t<_Mykey_t>, _STD add_pointer_t<_Mynode_t>, _STD add_pointer_t<_Mynode_t>>,
		"_Node must have constructor _Node(const _Val&, const _Key&, _Node*, _Node*) -- see _Default_node_t");

	static_assert(_STD is_same_v<_NSTD add_cr_t<_Val>, decltype(_STD declval<_STD add_const_t<_Mynode_t>>().GetVal(_STD declval<_NSTD add_cr_t<_Mykey_t>>()))>,
		"_Node must have nonstatic method const _Val& GetVal(const _Key&) const -- see _Default_node_t");



public:
	using key_type			= _Mykey_t;
	using value_type		= _Myval_t;
	using comparitor_type	= _Mycompare_t;
	using node_type			= _Mynode_t;

	template <typename array_type>
	BinTree(const array_type& arr) {
		// TODO
	}

	template <typename array_type>
		requires _Good_array<array_type>
	_NODISCARD _Mynode_t* genNode(const array_type& arr) const {
		
		

		
	}

	

	_NODISCARD const _Myval_t& GetVal(const _Mykey_t& key) {
		return _Head_node->GetVal(key);
	}

private:

	_HAS_CONCEPT(length); // concept _Has_length_v
	_HAS_CONCEPT(begin); // concept _Has_begin_v
	_HAS_CONCEPT(end); // concept _Has_end_v
	_HAS_CONCEPT(push, _STD declval<_Mykey_t>()); // concept _Has_push_v

	template <typename, typename = _STD void_t<>> struct _Has_bop : _STD false_type {};
	template <typename T> struct _Has_bop <T, _STD void_t<decltype(_STD declval<T>().operator[](_STD declval<_NSTD add_cr_t<_NSTD uint>>()))>> : _STD true_type {};	
	template <typename T> concept _Has_bop_v = _Has_length<T>::value;

	_HAS_CONCEPT(get_first); // concept _Has_get_first_v
	_HAS_CONCEPT(get_second); // concept _Has_get_second_v

	template <typename T> concept _Good_array = _Has_length_v<T> && _Has_begin_v<T> && _Has_end_v<T> && _Has_bop_v<T>	&&
		_Has_get_first_v<decltype(_STD declval<T>().operator[](_STD declval<_NSTD add_cr_t<_NSTD uint>>()))>			&&
		_Has_get_second_v<decltype(_STD declval<T>().operator[](_STD declval<_NSTD add_cr_t<_NSTD uint>>()))>;
	

	template <typename T> concept _Good_len = _STD convertible_to<T, _NSTD uint>;




	_Mynode_t* _Head_node;
};


_NSTD_END

#endif // _NSTD_BINTREE_