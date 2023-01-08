#pragma once
#ifndef _NSTD_TYPETRAITS_
#define _NSTD_TYPETRAITS_

#include <type_traits>
#include "Defines.h"

// Things I thought could be useful but aren't in <type_traits>
_NSTD_BEGIN

// Check if template compiles, if not, see error msg
template <typename> constexpr bool _Always_true = true;

template <typename T> struct add_cref { using type = const T&; };
template <typename T> using  add_cref_t = add_cref<T>::type;

template <typename T> constexpr bool _Is_generic = false;
template <template <typename...> typename T, typename... _Args> constexpr bool _Is_generic<T<_Args...>> = true;

template <typename T> concept _Rebindable = requires { _Is_generic<T>; };

template <typename T, typename... _Traits> struct rebind {};
template <template <typename...> typename T, typename... _Args, typename... _Traits> struct rebind<T<_Args...>, _Traits...> { using type = T<_Traits...>; };
template <typename T, typename... _Traits> requires _Rebindable<T> using rebind_t = rebind<T, _Traits...>::type;

template <typename T> struct remove_cref { using type = _STD remove_reference_t<_STD remove_const_t<T>>; };
template <typename T> using  remove_cref_t = remove_cref<T>::type;

_NSTD_END

#endif // _NSTD_TYPETRAITS_