#pragma once
#include "Threads.h"
#ifdef _NSTD_THREADS_

_NSTD_BEGIN

thread_pool& thread_pool::spin_up(uint count) {
	uint hc = _STD thread::hardware_concurrency();
	_Threads.resize(count >= hc ? hc - 1 : count);
	_Done = false;
	for(_STD thread& t : _Threads)
		t = _STD thread([this] { thread_loop(); });
	return *this;
}

thread_pool& thread_pool::release() {
	{
		_STD lock_guard<_STD mutex> lock(_QMutex);
		_Done = true;
	}
	_MutCond.notify_all();
	for (_STD thread& t : _Threads)
		t.join();
	_Threads.clear();
	return *this;
}

void thread_pool::thread_loop() {
	while (true) {
		_Pair_fvp task;
		{
			_STD unique_lock<_STD mutex> lock(_QMutex);
			_MutCond.wait(lock, [this] { return !_Tasks.empty() || _Done; });
			if (_Done && _Tasks.empty())
				return;
			task = _Tasks.front();
			_Tasks.pop_front();
		}
		task.first(task.second);
	}
}

void thread_pool::add_task(const _Func& func, void* data) {
	{
		_STD lock_guard<_STD mutex> lock(_QMutex);
		_Tasks.emplace_back(_Pair_fvp(func, data));
	}
	_MutCond.notify_one();
}

void thread_pool::add_task_inplace(const _Func& _Func, size_t _Index, void* data) {
	{
		_STD lock_guard<_STD mutex> l(_QMutex);
		_Tasks.insert(_Tasks.begin() + _Index, _Pair_fvp(_Func, data));
	}
	_MutCond.notify_one();
}

void thread_pool::add_task(_Pair_fvp& pair) {
	{
		_STD lock_guard<_STD mutex> lock(_QMutex);
		_Tasks.emplace_back(pair);
	}
	_MutCond.notify_one();
}

void thread_pool::add_task(_STD vector<_Pair_fvp> task_list) {
	{
		_STD lock_guard<_STD mutex> lock(_QMutex);
		for (_Pair_fvp& task : task_list)
			_Tasks.push_back(task);
	}
	_NSTD_FOR_I(task_list.size())
		_MutCond.notify_one();
}

_NSTD_END
#endif // _NSTD_THREADS_