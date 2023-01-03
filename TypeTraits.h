#pragma once
#ifndef _NSTD_TYPETRAITS_
#define _NSTD_TYPETRAITS_

#include <type_traits>
#include "Defines.h"

// Things I thought could be useful but aren't in <type_traits>
_NSTD_BEGIN

// Check if T compiles, if not, see error msg
template <typename T> struct _Always_true { using value = true; };
template <typename T> using  _Always_true_v = _Always_true<T>::value;

template <typename T> struct add_cr { using type = const remove_cr_t<T>&; };
template <typename T> using  add_cr_t = add_cr<T>::type;

template <typename T, typename... _Traits> struct rebind { using type = T<_Traits...>; };
template <typename T, typename... _Traits> using  rebind_t = rebind<T, _Traits...>::type;

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