#pragma once
#ifndef _NSTD_BINTREE_
#define _NSTD_BINTREE_

#include <utility>
#include <memory>
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
	using _Mykey_t		= _NSTD remove_crv_t<_Key>;
	using _Myval_t		= _NSTD remove_crv_t<_Val>;
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

	template <typename array_type, typename _Mysize_t = _STD size_t>
	BinTree(const array_type& arr, const _Mysize_t& size) {
		// TODO
	}

	template <typename array_type>
	_NODISCARD _NSTD rebind_t<array_type, _STD add_pointer_t<_Mynode_t>> genNodeVector(const array_type& arr) const {
		static_assert(_STD is_function_v<decltype(_STD declval<array_type>().length())>,
			"array_type must have method array_type::length() that produces compatible size_type");
		
		using _Mysize_t = decltype(_STD declval<array_type>().length());

		static_assert(_STD constructible_from<_Mysize_t, const _Mysize_t&>,
			"size_type (produced by array_type::length) must be constructible from a const size_type& object");
		
		_Mysize_t length_copy(arr.length());

		static_assert(_STD is_same_v<_Mysize_t&, decltype(--_STD declval<_Mysize_t&>())>,
			"size_type (produced by array_type::length) must have prefix decrement operator");
		static_assert(_STD is_convertable_v<_Mysize_t, bool>,
			"size_type (produced by array_type::length) must be convertable to a bool to indicate when it is zero");
		
		// See msg on compile error
		static_assert(_NSTD _Always_true_v<decltype(_STD declval<array_type>()[_STD declval<_Mysize_t>()])>,
			"array_type must be indexable by const size_type& using operator[](const size_type&)");
		static_assert(_NSTD _Always_true_v<decltype(_STD declval<array_type>().push(_STD declval<_Mykey_t>()))>,
			"array_type must have the ability to push key_type objects as a means of expanding its contents");
		//

		using _Myarr_ret_t = decltype(_STD declval<array_type>()[_STD declval<_Mysize_t>()]);
		
		static_assert(_STD is_same_v<_Myval_t, _NSTD remove_crv_t<decltype(_STD declval<_Myarr_ret_t>().get_first())>>,
			"the type that array_type carries must have a get_first method returning a value_type object");
		static_assert(_STD is_same_v<_Mykey_t, _NSTD remove_crv_t<decltype(_STD declval<_Myarr_ret_t>().get_second())>>,
			"the type that array_type carries must have a get_second method returning a key_type object");
		

		using _Myarr_t = _NSTD rebind_t<array_type, _STD add_pointer_t<_Mynode_t>>;
		_Myarr_t vector;

		while (--length_copy)
			vector.push(new _Mynode_t(arr[length_copy].get_first(), arr[length_copy].get_second(), nullptr, nullptr));

		return vector;
	}



	_NODISCARD const _Myval_t& GetVal(const _Mykey_t& key) {
		return _Head_node.GetVal(key);
	}

private:
	_Mynode_t* _Head_node;
};


_NSTD_END

#endif // _NSTD_BINTREE_