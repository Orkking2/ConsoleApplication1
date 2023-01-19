#pragma once

#ifndef _NSTD_THREADS_
#define _NSTD_THREADS_

#include <functional>
#include <vector>
#include <thread>
#include <deque>
#include <mutex>
#include <utility>
#include "Defines.h"

_NSTD_BEGIN

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
class thread_pool {
public:
	using _Func = _STD function<void(void*)>;
	using _Pair_fvp = _STD pair<_Func, void*>;

public:
	thread_pool() : _Done(true) {}
	thread_pool(uint count) {
		spin_up(count);
	}
	~thread_pool() { release(); }
	
	thread_pool& spin_up(uint = _STD thread::hardware_concurrency() - 1);
	thread_pool& release();

	_NODISCARD bool _Is_running() { return !_Done; }
	_NODISCARD uint num_threads() { return _Threads.size(); }

	void thread_loop();

	void add_task(const _Func&, void* = NULL);
	void add_task_inplace(const _Func&, size_t, void* = NULL);
	
	template <typename _Pr>
	void add_task(_Pr _Pred, void* p = NULL) {
		add_task(_Func(_Pred), p);
	}
	template <typename _Pr>
	void add_task_inplace(_Pr _Pred, size_t _Index, void* p = NULL) {
		add_task_inplace(_Func(_Pred), _Index, p);
	}

	void add_task(_Pair_fvp&);
	void add_task(_STD vector<_Pair_fvp>);

	template <class R, class... Args>
	void add_task(const _STD function<R(Args...)>& func, _STD mutex& tup_mutex, _STD tuple<R*, Args...>& data) {
		{
			_STD lock_guard<_STD mutex> lock(_QMutex);
			_Tasks.push_back(_Pair_fvp(make_thread_safe_TUPLE(func, tup_mutex), reinterpret_cast<void*> (&data)));
		}
		_MutCond.notify_one();
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

	// Evals functions in order
	struct _Ordered_functional {


		_STD deque<_Pair_fvp> _To_eval;
	};

private:
	_STD vector<_STD thread> _Threads;
	_STD deque<_Pair_fvp> _Tasks;
	_STD mutex _QMutex;
	_STD condition_variable _MutCond;
	bool _Done;
};

_NSTD_END
#endif // !_NSTD_THREADS_