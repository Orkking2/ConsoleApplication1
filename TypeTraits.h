#pragma once
#ifndef _NSTD_TYPETRAITS_
#define _NSTD_TYPETRAITS_

#include <type_traits>
#include "Defines.h"

// Things I thought could be useful but aren't in <type_traits>
_NSTD_BEGIN

// Check if template compiles, if not, see error msg
template <typename> constexpr bool _Always_true = true;

template <typename T> struct add_cr { using type = const T&; };
template <typename T> using  add_cr_t = add_cr<T>::type;

template <typename T> struct remove_template { using type = T; };
template <template <typename...> typename T, typename... _Args> struct remove_template<T<_Args...>> { using type = T; };
template <typename T> using remove_template_t = remove_template<T>::type;

template <template <typename...> typename T, typename... _Traits> struct rebind { using type = T<_Traits...>; };
template <template <typename...> typename T, typename... _Traits> using  rebind_t = rebind<T, _Traits...>::type;

template <typename T> struct remove_cref { using type = _STD remove_reference_t<_STD remove_const_t<T>>; };
template <typename T> using  remove_cref_t = remove_cref<T>::type;

_NSTD_END

#endif // _NSTD_TYPETRAITS_