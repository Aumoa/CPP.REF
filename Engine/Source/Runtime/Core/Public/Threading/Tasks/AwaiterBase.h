// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IAwaiter.h"
#include <memory>

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
		Then([this, coroutine, awaiter = coroutine.promise().GetAwaiter()](auto)
		{
			if (GetStatus() == ETaskStatus::Canceled || awaiter->IsCancellationRequested())
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

	virtual ETaskStatus GetStatus() const noexcept = 0;
	virtual void ContinueWith(std::function<void(std::shared_ptr<AwaiterBase>)> awaiter) = 0;
	virtual std::exception_ptr GetException() = 0;
	virtual void Wait() = 0;

	virtual void Cancel() = 0;
	virtual bool SetException(std::exception_ptr ptr) = 0;

	virtual void AddCancellationToken(std::stop_token sToken) = 0;
	virtual void AddConditionVariable(std::function<bool()> condition) = 0;
	virtual bool IsCancellationRequested() const noexcept = 0;

	inline bool IsCompleted() const noexcept
	{
		ETaskStatus s = GetStatus();
		return s == ETaskStatus::RanToCompletion
			|| s == ETaskStatus::Canceled
			|| s == ETaskStatus::Faulted;
	}
};