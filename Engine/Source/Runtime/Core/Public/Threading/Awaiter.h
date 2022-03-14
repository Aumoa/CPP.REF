// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IAwaiter.h"
#include "Misc/Exceptions.h"
#include <coroutine>
#include <mutex>
#include <future>
#include <queue>

template<class T>
class Awaiter : public IAwaiter
{
	std::mutex _lock;
	std::promise<T> _promise;
	std::shared_future<T> _future;
	ETaskStatus _status = ETaskStatus::Created;
	std::queue<std::function<void(Task<void>)>> _thens;
	std::exception_ptr _exception;
	bool _freezed = false;
	std::source_location _source;

public:
	Awaiter()
	{
		_future = _promise.get_future();
	}

	Awaiter(const Awaiter&) = delete;

	virtual ETaskStatus GetStatus() const override
	{
		return _status;
	}

	virtual bool IsCompleted() const override
	{
		return _status == ETaskStatus::RanToCompletion
			|| _status == ETaskStatus::Faulted
			|| _status == ETaskStatus::Canceled;
	}

	virtual bool IsCompletedSuccessfully() const override
	{
		return _status == ETaskStatus::RanToCompletion;
	}

	virtual bool IsCanceled() const override
	{
		return _status == ETaskStatus::Canceled;
	}

	virtual bool IsFaulted() const override
	{
		return _status == ETaskStatus::Faulted;
	}

	virtual void WaitingToRun() override
	{
		auto lock = std::unique_lock(_lock);
		_status = ETaskStatus::WaitingToRun;
	}

	virtual void Running() override
	{
		auto lock = std::unique_lock(_lock);
		_status = ETaskStatus::Running;
	}

	virtual void Wait() override
	{
		_future.wait();
	}

	virtual void Then(std::function<void(Task<void>)> proc) override
	{
		auto lock = std::unique_lock(_lock);
		if (IsCompleted())
		{
			lock.unlock();
			proc(Task<void>(shared_from_this()));
		}
		else
		{
			_thens.emplace(std::move(proc));
		}
	}

	virtual void Cancel(const std::source_location& source = std::source_location::current()) override
	{
		auto lock = std::unique_lock(_lock);
		if (_freezed)
		{
			throw invalid_operation("Task is freezed.");
		}
		if (!IsCompleted())
		{
			_status = ETaskStatus::Canceled;
			_exception = std::make_exception_ptr(task_canceled());
			_promise.set_exception(_exception);
			_source = source;

			lock.unlock();
			InvokeThens();
		}
		else
		{
			throw invalid_operation("Task already completed.");
		}
	}

	virtual void SetException(std::exception_ptr ptr, const std::source_location& source = std::source_location::current()) override
	{
		auto lock = std::unique_lock(_lock);
		if (_freezed)
		{
			throw invalid_operation("Task is freezed.");
		}
		if (!IsCompleted())
		{
			_status = ETaskStatus::Faulted;
			_promise.set_exception(ptr);
			_exception = ptr;
			_source = source;

			lock.unlock();
			InvokeThens();
		}
		else
		{
			throw invalid_operation("Task already completed.");
		}
	}

	virtual std::exception_ptr GetException()
	{
		auto lock = std::unique_lock(_lock);
		return _exception;
	}

	void SetResult(const std::source_location& source = std::source_location::current())
	{
		SetResultImpl(source);
	}

	template<class U>
	void SetResult(U&& result, const std::source_location& source = std::source_location::current())
	{
		SetResultImpl(source, std::forward<U>(result));
	}

	T GetResult() const
	{
		return _future.get();
	}

	std::source_location GetSourceCode() const
	{
		return _source;
	}

	void Freeze()
	{
		auto lock = std::unique_lock(_lock);
		_freezed = true;
	}

	T await_resume()
	{
		return _future.get();
	}

private:
	template<class... U>
	void SetResultImpl(const std::source_location& source, U&&... result)
	{
		auto lock = std::unique_lock(_lock);
		if (_freezed)
		{
			throw invalid_operation("Task is freezed.");
		}
		if (!IsCompleted())
		{
			_status = ETaskStatus::RanToCompletion;
			_promise.set_value(std::forward<U>(result)...);
			_source = source;

			lock.unlock();
			InvokeThens();
		}
		else
		{
			throw invalid_operation("Task already completed.");
		}
	}

	void InvokeThens()
	{
		for (; !_thens.empty(); _thens.pop())
		{
			Invoke(_thens.front());
		}
	}

	void Invoke(std::function<void(Task<void>)>& body)
	{
		body(Task<void>(shared_from_this()));
	}
};