#pragma once
#ifndef _NSTD_ANY_
#define _NSTD_ANY_

#include <any>
#include "Defines.h"
#include "TypeTraits.h"


template <class _Interface>
struct __get_interface_ptr {
	using pointer = _STD add_pointer_t<_Interface>;
};
template <class _Interface> using __get_interface_ptr_p = __get_interface_ptr<_Interface>::pointer;



template <class _Interface>
struct __clone_base { // add clone() as necessarily implemented method
	virtual __get_interface_ptr_p<_Interface> clone() const = 0;
};

template <class _Interface>
struct __pointed_to {
	virtual __get_interface_ptr_p<_Interface> operator->() = 0;
};

//template <class _Class>
//concept __cloneable = requires(_STD add_pointer_t<_Class> p) {
//	{ p->clone() } -> _STD same_as<_STD add_pointer_t<_Class>>;
//}

template <class _Interface, typename _Under_t>
	//requires __cloneable<_Interface>
class __any_container_base : public _Interface, public __clone_base<_Interface> {
	
public:
	using interface_t = _Interface;
	using pointer = __get_interface_ptr_p<_Interface>;
	using under_t = _Under_t;

public:
	__any_container_base(const under_t& other) : _under(other) {}

	pointer clone() const override {
		return new __any_container_base(_under);
	}

protected:
	under_t& _getUnder() {
		return _under;
	}

private:
	under_t _under;
};

template <typename _Intermediary>
struct __any_size_t_interface : public __clone_base<decltype(this)> {
	using pointer = __get_interface_ptr_p<decltype(this)>;
	using intermediary = _STD remove_cvref_t<_Intermediary>;

	virtual ~__any_size_t_interface() = default;
	virtual pointer operator+ (const intermediary&) const = 0;
	virtual pointer operator+=(const intermediary&) = 0;
	virtual pointer operator- (const intermediary&) const = 0;
	virtual pointer operator-=(const intermediary&) = 0;
	virtual operator intermediary() const = 0;
	pointer operator->() = delete;
};

template <typename _Size_t, typename _Intermediary = _STD size_t>
class __any_size_t_container : public __any_container_base<__any_size_t_interface<_Intermediary>, _Size_t> {
	using _Contbase = __any_container_base<__any_size_t_interface<_Intermediary>, _Size_t>;
	using _Myinterface = _Contbase::interface_t;

public:
	using pointer = _Contbase::pointer;
	using intermediary = _Myinterface::intermediary;

public:
	pointer operator+(const intermediary& inter) const override {
		return new pointer(_Contbase::_getUnder() + inter);
	}
	pointer operator+=(const intermediary& inter) override {
		return new pointer(_Contbase::_getUnder() += inter)
	}
	pointer operator-(const intermediary& inter) const override {
		return new pointer(_Contbase::_getUnder() - inter);
	}
	pointer operator-=(const intermediary& inter) override {
		return new pointer(_Contbase::_getUnder() -= inter);
	}
	operator intermediary() const override {
		return static_cast<intermediary>(_Contbase::_getUnder());
	}
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