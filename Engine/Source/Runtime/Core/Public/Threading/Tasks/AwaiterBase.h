// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IAwaiter.h"
#include "suspend_and_destroy_if.h"
#include "TimeSpan.h"
#include "DateTime.h"
#include "Exceptions/InvalidOperationException.h"
#include <memory>
#include <any>

class AwaiterBase : public std::enable_shared_from_this<AwaiterBase>
{
protected:
	inline AwaiterBase() noexcept
	{
	}

public:
	virtual ~AwaiterBase() noexcept
	{
	}

	bool await_ready() noexcept
	{
		return IsCompleted();
	}

	template<class TCoroutineHandle>
	void await_suspend(TCoroutineHandle&& coroutine)
	{
		ContinueWith([this, coroutine, awaiter = coroutine.promise().GetAwaiter()](auto)
		{
			if (awaiter->IsCancellationRequested())
			{
				awaiter->Cancel();
				coroutine.destroy();
			}
			else
			{
				coroutine.resume();
			}
		});
	}

	void await_resume()
	{
		Wait();
		if (auto exception = GetException())
		{
			std::rethrow_exception(exception);
		}
	}

	virtual ETaskStatus GetStatus() const noexcept = 0;
	virtual void ContinueWith(std::function<void(std::shared_ptr<AwaiterBase>)> awaiter) = 0;
	virtual std::exception_ptr GetException() = 0;
	virtual void Wait() noexcept = 0;
	virtual bool WaitFor(const TimeSpan& timeout) noexcept = 0;
	virtual bool WaitUntil(const DateTime& timeout) noexcept = 0;

	virtual void Cancel() = 0;
	virtual void SetResult() { throw InvalidOperationException(TEXT("A typed task cannot setting result without value.")); }
	virtual bool SetException(std::exception_ptr ptr) = 0;

	virtual suspend_and_destroy_if AddCancellationToken(std::stop_token sToken) = 0;
	virtual suspend_and_destroy_if AddConditionVariable(std::function<bool()> condition) = 0;
	virtual void AddStopCallback(std::stop_token cancellationToken, std::function<void()> callback) = 0;
	virtual bool IsCancellationRequested() const noexcept = 0;

	inline bool IsCompleted() const noexcept
	{
		ETaskStatus s = GetStatus();
		return s == ETaskStatus::RanToCompletion
			|| s == ETaskStatus::Canceled
			|| s == ETaskStatus::Faulted;
	}
};