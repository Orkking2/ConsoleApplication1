#pragma once
#ifndef _NSTD_TYPETRAITS_
#define _NSTD_TYPETRAITS_

#include <type_traits>
#include "Defines.h"

// Things I thought could be useful but aren't in <type_traits>
_NSTD_BEGIN

// Check if template compiles, if not, see error msg (Utility ?)
template <typename> constexpr bool _Always_true = true;

template <typename _Ty> struct add_cref { using type = const _Ty&; };
template <typename _Ty> using  add_cref_t = add_cref<_Ty>::type;

template <typename _Ty> constexpr bool _Is_generic = false;
template <template <typename...> typename _Ty, typename... _Args> constexpr bool _Is_generic<_Ty<_Args...>> = true;

template <typename _Ty> concept _Rebindable = requires { _Is_generic<_Ty>; };

template <typename _Ty, typename... _Traits> struct rebind {};
template <template <typename...> typename _Ty, typename... _Args, typename... _Traits> struct rebind<_Ty<_Args...>, _Traits...> { using type = _Ty<_Traits...>; };
template <typename _Ty, typename... _Traits> requires _Rebindable<_Ty> using rebind_t = rebind<_Ty, _Traits...>::type;

template <typename _Ty> struct remove_cref { using type = _STD remove_reference_t<_STD remove_const_t<_Ty>>; };
template <typename _Ty> using  remove_cref_t = remove_cref<_Ty>::type;

_NSTD_END

#endif // _NSTD_TYPETRAITS_