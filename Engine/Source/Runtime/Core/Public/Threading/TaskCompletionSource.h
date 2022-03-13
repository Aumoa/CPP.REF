// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Awaiter.h"
#include "Task.h"

template<class T = void>
class TaskCompletionSource
{
	std::shared_ptr<Awaiter<T>> _awaiter;

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

	template<class... U>
	void SetResult(U&&... result)
	{
		Xassert(IsValid(), "Awaiter is null.");
		_awaiter->SetResult(std::forward<U>(result)...);
	}

	void SetException(std::exception_ptr ptr)
	{
		Xassert(IsValid(), "Awaiter is null.");
		_awaiter->SetException(std::move(ptr));
	}

	void SetCanceled()
	{
		Xassert(IsValid(), "Awaiter is null.");
		_awaiter->Cancel();
	}

	Task<T> GetTask()
	{
		return Task<T>(_awaiter);
	}

	TaskCompletionSource& operator =(const TaskCompletionSource&) = default;
	TaskCompletionSource& operator =(TaskCompletionSource&&) = default;

	template<class U = T>
	static TaskCompletionSource<U> Create()
	{
		return TaskCompletionSource<U>(std::make_shared<Awaiter<U>>());
	}

private:
	void Xassert(bool x, std::string_view message)
	{
		if (!x)
		{
			throw invalid_operation(message);
		}
	}
};