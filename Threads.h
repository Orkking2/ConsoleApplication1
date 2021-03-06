#pragma once

#ifndef _NSTD_THREADS_
#define _NSTD_THREADS_

#include "Defines.h"
#include <functional>
#include <utility>
#include <vector>
#include <thread>
#include <tuple>
#include <deque>
#include <mutex>

_NSTD_BEGIN

//template < class R, class... Args >
//struct func_wrapper {
//	func_wrapper(_STD function <R(Args...)> func, _STD mutex& mut, R* ret = NULL) : func_(func), mut_(mut), ret_(ret) {}
//		
//	void operator() (Args... args) {
//		_STD lock_guard<_STD mutex> ret_guard(mut_);
//		if (ret_)
//			*ret_ = func_(args);
//		else
//			func_(args);
//	}
//
//	_STD function<R(Args...)> func_;
//	_STD mutex mut_;
//	R* ret_;
//};

class thread_pool {
public:
	typedef _NSTD uint uint;

	typedef _STD function<void(void*)> _Func;
	typedef _STD pair <_Func, void*> _Pair_fvp;
	/* NOTE:
	* The use of void* as a parameter for each function is done so for abstraction.
	*
	* Each function must still take void* as a parameter, even if it is not used
	* within the function.
	*
	* If the void* parameter is not used, it is recomended to set the associated
	* data element to NULL or nullptr and to simply ignore it within the function.
	*
	* Functions passed into this thread pool should reinterpret the void*, e.g.
	* reinterpret_vast<T*> (void* p), before use.
	*
	* Object(s) pointed to by the void* pointer is/are NOT protected for
	* multithreading by default.
	*
	* Function(s) passed into the pool should only ever read from the pointer
	* or use a custom system to protect its/their contents from curroption;
	* see make_thread_safe function below.
	*/
private:
	_STD vector<_STD thread> vThreads_;
	_STD deque<_Pair_fvp> task_queue_;
	_STD mutex queue_mutex_;
	_STD condition_variable mutex_condition_;
	bool done_;
public:
	thread_pool(uint count = _STD thread::hardware_concurrency() - 1) : done_(false) {
		uint hc = _STD thread::hardware_concurrency();
		vThreads_.resize((count >= hc) ? hc - 1: count);
		for (_STD thread& t : vThreads_)
			t = _STD thread([this] { thread_loop(); });
	}
	void release();
	~thread_pool() { release(); }

	_NODISCARD uint num_threads() { return vThreads_.size(); }

	void thread_loop();

	void add_task(const _Func&, void* = NULL);
	void add_task(_Pair_fvp&);
	void add_task(_STD vector<_Pair_fvp>);

	template <class R, class... Args>
	void add_task(const _STD function<R(Args...)>& func, _STD mutex& tup_mutex, _STD tuple<R*, Args...>& data) {
		{
			_STD lock_guard<_STD mutex> lock(queue_mutex_);
			task_queue_.push_back(_Pair_fvp(make_thread_safe_TUPLE(func, tup_mutex), reinterpret_cast<void*> (&data)));
		}
		mutex_condition_.notify_one();
	}
	template <class R, class... Args>
	void add_task(const _STD function<R(Args...)>& func, _STD mutex& ret_mutex, R* ret, Args&&... args) {
		_STD tuple<R*, Args...> t = _STD make_tuple(ret, args...);
		add_task(make_thread_safe_TUPLE(func, ret_mutex), t);
	}


	// Function wrappers;

	template <class R, class... Args>
	_NODISCARD _Func make_thread_safe_TUPLE(const _STD function<R(Args...)>& func, _STD mutex& ret_mutex) {
		return _Func(
			[&func, &ret_mutex](void* p) {
				_STD lock_guard<_STD mutex> tuple_guard(ret_mutex);
				auto* t = reinterpret_cast<_STD tuple<R*, Args...>*>(p);
				*_STD get<R*>(*t) = func(_STD get<Args>(*t) ...);
			}
		);
	}

	template <class... Args>
	_NODISCARD _Func make_thread_safe_TUPLE_NORETURN(const _STD function<void(Args...)>& func) {
		return _Func(
			[&func](void* p) {
				auto* t = reinterpret_cast<_STD tuple<Args...>*>(p);
				func(_STD get<Args>(*t) ...);
			}
		);
	}

	template <class... Args>
	_NODISCARD _Func make_thread_safe_TUPLE_NORETURN_DELETE_PTR(const _STD function<void(Args...)>& func) {
		return _Func(
			[&func](void* p) {
				auto* t = reinterpret_cast<_STD tuple<Args...>*>(p);
				func(_STD get<Args>(*t) ...);
				delete t;
			}
		);
	}
};

_NSTD_END
#endif // !_NSTD_THREADS_