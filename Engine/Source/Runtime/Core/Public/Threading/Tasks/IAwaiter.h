// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "TaskStatus.h"
#include <memory>
#include <functional>
#include <source_location>

namespace libty::inline Core
{
	template<class T>
	class Task;

	class IAwaiter : public std::enable_shared_from_this<IAwaiter>
	{
	protected:
		IAwaiter() = default;

	public:
		virtual ~IAwaiter() noexcept = default;

		virtual ETaskStatus GetStatus() const = 0;
		virtual bool IsCompleted() const = 0;
		virtual bool IsCompletedSuccessfully() const = 0;
		virtual bool IsCanceled() const = 0;
		virtual bool IsFaulted() const = 0;

		virtual void WaitingToRun() = 0;
		virtual void Running() = 0;
		virtual void Wait() = 0;
		virtual void Then(std::function<void(Task<void>)> proc) = 0;
		virtual void Cancel(std::source_location source = std::source_location::current()) = 0;

		virtual void SetException(std::exception_ptr ptr, std::source_location source = std::source_location::current()) = 0;
		virtual std::exception_ptr GetException() = 0;
		virtual std::source_location GetCompletionSource() = 0;

		bool await_ready()
		{
			return IsCompleted();
		}

		template<class TCoroutineHandle>
		void await_suspend(TCoroutineHandle&& coroutine)
		{
			Then([this, coroutine, awaiter = coroutine.promise().GetAwaiter()](auto)
			{
				if (IsCanceled() || awaiter->_Invoke_cancel_requested())
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

	protected:
		virtual bool _Cancel_requested() = 0;

	private:
		inline bool _Invoke_cancel_requested()
		{
			return this->_Cancel_requested();
		}
	};
}