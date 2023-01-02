#pragma once
#ifndef _NSTD_TYPETRAITS_
#define _NSTD_TYPETRAITS_

#include <type_traits>
#include "Defines.h"

// Things I thought could be useful but aren't in <type_traits>
_NSTD_BEGIN

template <typename _Ty>
struct add_cr {
	using type = const remove_cr_t<_Ty>&;
};

template <typename _Ty>
using add_cr_t = add_cr<_Ty>::type;

template <typename _Ty, typename... _Traits>
struct rebind {
	using type = _Ty<_Traits...>;
};

template <typename _Ty, typename... _Traits>
using rebind_t = rebind<_Ty, _Traits...>::type;

template <typename _Ty, typename _Kty, typename _Rty>
constexpr bool is_indexable_by() {
    return _STD is_class<_Ty>::value && 
        _STD is_function<decltype(_STD declval<_Ty>()[_STD declval<_Kty>()])>::value &&
        _STD is_same_v<decltype(_STD declval<_Ty>()[_STD declval<_Kty>()]), _Rty>;
}

template <typename T> struct remove_reference     { using type = T; };
template <typename T> struct remove_reference<T&> { using type = T; };
template <typename T> using  remove_reference_t = remove_reference<T>::type;

template <typename T> struct remove_cr { using type = remove_reference_t<_STD remove_const_t<T>>; };
template <typename T> using  remove_cr_t = remove_cr<T>::type;

template <typename T> struct remove_rv { using type = remove_reference_t<_STD remove_volatile_t<T>>; };
template <typename T> using  remove_rv_t = remove_rv<T>::type;

template <typename T> struct remove_crv { using type = remove_reference_t<_STD remove_cv_t<T>>; };
template <typename T> using  remove_crv_t = remove_crv<T>::type;

_NSTD_END

#endif // _NSTD_TYPETRAITS_