#pragma once
#ifndef _NSTD_BINTREE_
#define _NSTD_BINTREE_

#include <type_traits>
#include <utility>
#include "Defines.h"

_NSTD_BEGIN

template <typename Key, typename Val = void*>
class BinTree {

	template <typename T>
	struct has_compare {
		template <typename U>
		static auto test(int) -> decltype(_STD is_same_v<_STD strong_ordering, decltype(_STD declval<const U>()(_STD declval<const Key&>(), _STD declval<const Key&>()) const)>, _STD true_type());

		template <typename>
		static _STD false_type test(...);

		static constexpr bool value = decltype(test<T>(0))::value;
	};
	template <typename T>
	using CompareType = _STD enable_if_t<has_compare<T>::value, T>;



	struct Element {
		// Will make a copy of Val input
		Element(const Val& v, Element* l, Element* r) : _val(v), _left(l), _right(r) {}

		~Element() {

		}
		// T must have: _STD strong_ordering T.operator()(const Key&, const Key&);
		template <typename T, typename = CompareType<T>>
		const Val& GetVal(const Key&, const T&) const {

		}

		Val _val;
		Element* _left, * _right;
	};

};


_NSTD_END

#endif // _NSTD_BINTREE_