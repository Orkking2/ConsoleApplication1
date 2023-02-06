#pragma once
#ifndef _NSTD_ANY_
#define _NSTD_ANY_

#include <xutility>
#include <concepts>
#include <any>
#include "Defines.h"
#include "TypeTraits.h"


_NSTD_BEGIN

template <class _Interface>
struct __get_interface_ptr {
	using pointer = _STD add_pointer_t<_Interface>;
};
template <class _Interface> using __get_interface_ptr_p = __get_interface_ptr<_Interface>::pointer;

template <class _Class>
concept __cloneable = _STD convertible_to<__get_interface_ptr_p<_Class>, decltype(_STD declval<__get_interface_ptr_p<_Class>>()->clone())>;

template <class _Interface>
struct __clone_base { // add clone() as necessarily implemented method
	virtual __get_interface_ptr_p<_Interface> clone() const = 0;
};

template <class _Interface>
struct __pointed_to {
	virtual __get_interface_ptr_p<_Interface> operator->() = 0;
};

template <class _Interface, typename _Under_t>
class __any_container_base : public _Interface, public __clone_base<_Interface> {
	
public:
	using interface_t = _Interface;
	using pointer = __get_interface_ptr_p<_Interface>;
	using under_t = _Under_t;

public:
	__any_container_base(const under_t& other) : _under(other) {}

	//pointer clone() const override {
	//	return new __any_container_base(_under);
	//}

protected:
	under_t& _getUnder() {
		return _under;
	}
	const under_t& _getUnder() const {
		return _under;
	}

private:
	under_t _under;
};

template <class T>
struct __any_base {
	static_assert(_STD _Always_false<T>,
		"__any_base invalid params.");
};

template <template <typename, typename...> class _Container, typename _Default_val, typename... _Traits>
class __any_base<_Container<_Default_val, _Traits...>> {
	using _Mycontainer = _Container<_Default_val, _Traits...>;

	template <typename T>
	using __replaced_container = _Container<T, _Traits...>;

public:
	using container = _Mycontainer;
	using interface_t = _Mycontainer::interface_t;
	using pointer = _Mycontainer::pointer;

public:
	__any_base() : _contents(nullptr) {}
	template <typename _Base>
	__any_base(const _Base& base) : _contents(new __replaced_container<_Base>(base)) {}
	__any_base(const __any_base& other) : _contents(other->clone()) {}

	template <typename _Base>
	__any_base& operator= (const _Base& base) {
		clear();
		_contents = new __replaced_container<_Base>(base);
	}
	__any_base& operator= (const __any_base& other) {
		clear();
		_contents = other->clone();
	}

	pointer operator->() {
		return _contents;
	}

	bool empty() const {
		return _contents == nullptr;
	}

	void clear() {
		delete _contents;
		_contents = nullptr;
	}

	~__any_base() {
		clear();
	}

protected:
	pointer& _getContents() {
		return _contents;
	}

private:
	pointer _contents;
};

template <typename _Intermediary>
struct __any_size_t_interface {
	using pointer = __get_interface_ptr_p<__any_size_t_interface<_Intermediary>>;
	using intermediary = _STD remove_cvref_t<_Intermediary>;

	virtual ~__any_size_t_interface() = default;
	
	virtual pointer operator+ (const intermediary&) const = 0;
	virtual pointer operator+=(const intermediary&) = 0;
	virtual pointer operator++(int) = 0;
	virtual pointer operator++() = 0;
	template <typename T> pointer operator+ (const T& t) const { return operator+ (static_cast<intermediary>(t)); }
	template <typename T> pointer operator+=(const T& t)       { return operator+=(static_cast<intermediary>(t)); }

	virtual pointer operator- (const intermediary&) const = 0;
	virtual pointer operator-=(const intermediary&) = 0;
	virtual pointer operator--(int) = 0;
	virtual pointer operator--() = 0;
	template <typename T> pointer operator- (const T& t) const { return operator- (static_cast<intermediary>(t)); }
	template <typename T> pointer operator-=(const T& t)       { return operator-=(static_cast<intermediary>(t)); }
	
	virtual operator intermediary() const = 0;
	template <typename T> operator T() const { return static_cast<T>(operator intermediary()); }

	pointer operator->() = delete;
};

template <typename _Size_t, typename _Intermediary>
class __any_size_t_container : public __any_container_base<__any_size_t_interface<_Intermediary>, _Size_t> {
	using _Contbase = __any_container_base<__any_size_t_interface<_Intermediary>, _Size_t>;

public:
	using interface_t = _Contbase::interface_t;
	using pointer = _Contbase::pointer;
	using under_t = _Contbase::under_t;
	using intermediary = interface_t::intermediary;

public:
	template <typename T>
	__any_size_t_container(const T& t) : _Contbase(t) {}

	pointer clone() const override {
		return new __any_size_t_container(_Contbase::_getUnder());
	}

	pointer operator+(const intermediary& inter) const override {
		return new __any_size_t_container(_Contbase::_getUnder() + inter);
	}
	pointer operator+=(const intermediary& inter) override {
		return new __any_size_t_container(_Contbase::_getUnder() += inter);
	}
	pointer operator++(int) override { // postfix
		return new __any_size_t_container(_Contbase::_getUnder()++);
	}
	pointer operator++() override { // prefix
		++_Contbase::_getUnder();
		return this;
	}

	pointer operator-(const intermediary& inter) const override {
		return new __any_size_t_container(_Contbase::_getUnder() - inter);
	}
	pointer operator-=(const intermediary& inter) override {
		return new __any_size_t_container(_Contbase::_getUnder() -= inter);
	}
	pointer operator--(int) override { // postfix
		return new __any_size_t_container(_Contbase::_getUnder()--);
	}
	pointer operator--() override { // prefix
		--_Contbase::_getUnder();
		return this;
	}
	operator intermediary() const override {
		return static_cast<intermediary>(_Contbase::_getUnder());
	}
};

template <typename _Intermediary = _STD size_t>
class __any_size_t : public __any_base<__any_size_t_container<_STD size_t /* default */, _Intermediary>> {
	using __base = __any_base<__any_size_t_container<_STD size_t, _Intermediary>>;

public:
	using interface_t = __base::interface_t;
	using pointer = __base::pointer;
	using intermediary = __base::container::intermediary;

public:
	template <typename T> 
	__any_size_t operator+ (const T& t) { 
		return __any_size_t(__base::_getContents()->operator+ (t)); 
	}
	template <typename T> 
	__any_size_t& operator+=(const T& t) { 
		__base::_getContents()->operator+=(t); 
		return *this; 
	}
	template <typename T> 
	__any_size_t operator- (const T& t) { 
		return __any_size_t(__base::_getContents()->operator- (t)); 
	}
	template <typename T> 
	__any_size_t& operator-=(const T& t) {
		__base::_getContents()->operator-=(t); 
		return *this; 
	}
	template <typename T> 
	operator T() { 
		return __base::_getContents()->operator T(); 
	}
};

template <typename _Ret, typename _Intermediary>
struct __any_iterator_interface {
	using value_type = _Ret;
	using intermediary = _Intermediary;
	using pointer = __get_interface_ptr_p<__any_iterator_interface<_Ret>>;

	virtual ~__any_iterator_interface() = default;
	virtual pointer operator+ (const __any_size_t<intermediary>&) const = 0;
	virtual pointer operator+=(const __any_size_t<intermediary>&) = 0;
	virtual pointer operator++(int) = 0;
	virtual pointer operator++() = 0;

	virtual pointer operator- (const __any_size_t<intermediary>&) const = 0;
	virtual pointer operator-=(const __any_size_t<intermediary>&) = 0;
	virtual pointer operator--(int) = 0;
	virtual pointer operator--() = 0;

	virtual value_type operator*() const = 0;
	virtual value_type operator->() = 0;
};

template <class _Iter, typename _Ret = typename _Iter::value_type, typename _Intermediary = typename _Iter::size_type>
class __any_iterator_container : __any_container_base<__any_iterator_interface<_Ret, _Intermediary>, _Iter> {
	using _Contbase = __any_iterator_interface<_Ret>;

public:
	using interface_t = _Contbase::interface_t;
	using pointer = _Contbase::pointer;
	using under_t = _Contbase::under_t;
	using intermediary = interface_t::intermediary;

public:
	pointer operator+ (const __any_size_t<_Intermediary>& size) const override {
		return new __any_iterator_container(
			_Contbase::_getUnder() + static_cast<_Intermediary>(size)
		);
	}
	pointer operator+=(const __any_size_t<_Intermediary>& size) override {
		_Contbase::_getUnder() += static_cast<_Intermediary>(size);
		return this;
	}
	pointer operator++(int) override {
		return new __any_iterator_container(
			_Contbase::_getUnder()++
		);
	}
	pointer operator++() override {
		++_Contbase::_getUnder();
		return this;
	}

	pointer operator- (const __any_size_t<_Intermediary>& size) const override {
		return new __any_iterator_container(
			_Contbase::_getUnder() - static_cast<_Intermediary>(size)
		);
	}
	pointer operator-=(const __any_size_t<_Intermediary>& size) override {
		_Contbase::_getUnder() -= static_cast<_Intermediary>(size);
		return this;
	}
	pointer operator--(int) override {
		return new __any_iterator_container(
			_Contbase::_getUnder()--
		);
	}
	pointer operator--() override {
		--_Contbase::_getUnder();
		return this;
	}
};

_NSTD_END
#endif // _NSTD_ANY_