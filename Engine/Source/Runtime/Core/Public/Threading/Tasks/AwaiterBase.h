// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/TaskStatus.h"
#include "Threading/Tasks/suspend_and_destroy_if.h"
#include "Action.h"
#include "TimeSpan.h"
#include "DateTime.h"
#include "InvalidOperationException.h"
#include <memory>

namespace Ayla
{
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
		void await_suspend(TCoroutineHandle&& Coro)
		{
			ContinueWith([this, Coro, Awaiter = Coro.promise().GetAwaiter()](auto)
				{
					if (Awaiter->IsCancellationRequested())
					{
						Awaiter->Cancel();
						Coro.destroy();
					}
					else
					{
						Coro.resume();
					}
				});
		}

		void await_resume()
		{
			Wait();
			if (auto ExceptionPtr = GetException())
			{
				std::rethrow_exception(ExceptionPtr);
			}
		}

		virtual TaskStatus GetStatus() const noexcept = 0;
		virtual void ContinueWith(Action<std::shared_ptr<AwaiterBase>> ContinuationBody) = 0;
		virtual std::exception_ptr GetException() = 0;
		virtual void Wait() noexcept = 0;
		virtual bool WaitFor(const TimeSpan& Timeout) noexcept = 0;
		virtual bool WaitUntil(const DateTime& Timeout) noexcept = 0;

		virtual void Cancel() = 0;
		virtual void SetResult() { throw InvalidOperationException(TEXT("A typed task cannot setting result without value.")); }
		virtual bool SetException(std::exception_ptr ExceptionPtr) = 0;

		virtual suspend_and_destroy_if AddCancellationToken(CancellationToken sToken) = 0;
		virtual suspend_and_destroy_if AddConditionVariable(std::function<bool()> ConditionBody) = 0;
		virtual void AddStopCallback(CancellationToken sToken, std::function<void()> CallbackBody) = 0;
		virtual bool IsCancellationRequested() const noexcept = 0;

		inline bool IsCompleted() const noexcept
		{
			TaskStatus Status = GetStatus();
			return Status == TaskStatus::RanToCompletion
				|| Status == TaskStatus::Canceled
				|| Status == TaskStatus::Faulted;
		}
	};
}