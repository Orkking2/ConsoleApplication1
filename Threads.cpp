#pragma once
#include "Threads.h"
#ifdef _NSTD_THREADS_

_NSTD_BEGIN

void thread_pool::release() {
	{
		_STD lock_guard<_STD mutex> lock(queue_mutex_);
		done_ = true;
	}
	mutex_condition_.notify_all();
	for (_STD thread& thread : vThreads_)
		thread.join();
	vThreads_.clear();
}

void thread_pool::thread_loop() {
	while (true) {
		_Pair_fvp task;
		{
			_STD unique_lock<_STD mutex> lock(queue_mutex_);
			mutex_condition_.wait(lock, [this] { return !task_queue_.empty() || done_; });
			if (done_ && task_queue_.empty())
				return;
			task = task_queue_.front();
			task_queue_.pop_front();
		}
		task.first(task.second);
	}
}

void thread_pool::add_task(const _STD function<void(void*)>& func, void* data = NULL) {
	{
		_STD lock_guard<_STD mutex> lock(queue_mutex_);
		task_queue_.emplace_back(_Pair_fvp(func, data));
	}
	mutex_condition_.notify_one();
}

void thread_pool::add_task(_Pair_fvp& pair) {
	{
		_STD lock_guard<_STD mutex> lock(queue_mutex_);
		task_queue_.emplace_back(pair);
	}
	mutex_condition_.notify_one();
}

void thread_pool::add_task(_STD vector<_Pair_fvp> task_list) {
	{
		_STD lock_guard<_STD mutex> lock(queue_mutex_);
		for (_Pair_fvp& task : task_list)
			task_queue_.push_back(task);
	}
	_NSTD_FOR(task_list.size())
		mutex_condition_.notify_one();
}

_NSTD_END
#endif // _NSTD_THREADS_