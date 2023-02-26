// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Awaiter.h"
#include "Task.h"
#include "CoreTypes/String.h"
#include <source_location>

template<class T = void>
class TaskCompletionSource
{
	template<class>
	friend class TaskCompletionSource;

	std::shared_ptr<Awaiter<T>> _awaiter;
	std::source_location _set = std::source_location::current();

private:
	TaskCompletionSource(std::shared_ptr<Awaiter<T>> awaiter)
		: _awaiter(std::move(awaiter))
	{
	}

public:
	TaskCompletionSource() = default;
	TaskCompletionSource(const TaskCompletionSource&) = default;
	TaskCompletionSource(TaskCompletionSource&&) = default;

	bool IsValid() const
	{
		return (bool)_awaiter;
	}

	void SetResult(std::source_location src = std::source_location::current())
	{
		SetResultImpl();
		_set = src;
	}

	template<class U>
	void SetResult(U&& result, std::source_location src = std::source_location::current())
	{
		SetResultImpl(std::forward<U>(result));
		_set = src;
	}

	template<class TException>
	void SetException(const TException& e)
	{
		SetException(std::make_exception_ptr(e));
	}

	void SetException(std::exception_ptr ptr)
	{
		Xassert(IsValid(), TEXT("Awaiter is null."));
		_awaiter->SetException(std::move(ptr));
	}

	void SetCanceled()
	{
		Xassert(IsValid(), TEXT("Awaiter is null."));
		_awaiter->Cancel();
	}

	Task<T> GetTask()
	{
		return Task<T>(_awaiter);
	}

	TaskCompletionSource& operator =(const TaskCompletionSource&) = default;
	TaskCompletionSource& operator =(TaskCompletionSource&&) = default;

	template<class U = T>
	static TaskCompletionSource<U> Create(std::stop_token cancellationToken = {})
	{
		return TaskCompletionSource<U>(std::make_shared<Awaiter<U>>(cancellationToken));
	}

private:
	template<class... U>
	void SetResultImpl(U&&... result)
	{
		Xassert(IsValid(), TEXT("Awaiter is null."));
		_awaiter->SetResult(std::forward<U>(result)...);
	}

	void Xassert(bool x, String message)
	{
		if (!x)
		{
			throw InvalidOperationException(message);
		}
	}
};