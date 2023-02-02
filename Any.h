#pragma once
#ifndef _NSTD_ANY_
#define _NSTD_ANY_

#include <any>
#include "Defines.h"
#include "TypeTraits.h"

template <class _Base>
struct __clone_base { // add clone() as necessarily implemented method
	using pointer = _STD add_pointer_t<_Base>;
	virtual pointer clone() const = 0;
};

//template <class _Class>
//concept __cloneable = requires(_STD add_pointer_t<_Class> p) {
//	{ p->clone() } -> _STD same_as<_STD add_pointer_t<_Class>>;
//}

template <class _Interface>
	//requires __cloneable<_Interface>
class __any_base : _Interface, __clone_base<_Interface> {
	using _Cbase = __clone_base<_Interface>;

public:
	using pointer = _Cbase::pointer;

public:
	__any_base() : _data(0) {}
	__any_base(pointer data) : _data(data->clone()) {}
	__any_base(const __any_base& other) : _data(other.clone()) {}

	pointer clone() { // (*this)->clone()
		if(!_data)
			return 0;
		return _data->clone();
	}
	
	pointer operator->() { // (*this)->op <=> ((*this)->)->op
		return _data;
	}


private:
	pointer _data;
};


template <typename _Ret>
struct __any_iterator_interface : __clone_base<decltype(this)> {
	using value_type = _Ret;

	virtual ~__any_iterator_interface() = default;
	virtual decltype(this) operator+ (const decltype(this)&) const = 0;
	virtual decltype(this) operator+=(const decltype(this)&) = 0;
	virtual decltype(this) operator++(int) = 0;
	virtual decltype(this) operator++() = 0;

	virtual decltype(this) operator- (const decltype(this)&) const = 0;
	virtual decltype(this) operator-=(const decltype(this)&) = 0;
	virtual decltype(this) operator--(int) = 0;
	virtual decltype(this) operator--() = 0;

	virtual value_type operator*() const = 0;
};

template <class _Iter, typename _Ret = typename _Iter::value_type>
class __any_iterator_container : __any_iterator_base<__any_iterator_ {
	using __base = __any_iterator_interface<_Ret>;
	using __iter = _STD remove_cvref_t<_Iter>;

public:
	using value_type = __base::value_type;






private:
	__iter _contents;
};


#endif // _NSTD_ANY_