// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/Task.h"

namespace Ayla
{
	template<class T = void>
	class TaskCompletionSource
	{
		template<class>
		friend class TaskCompletionSource;

		std::shared_ptr<Awaiter<T>> MyAwaiter;
		mutable std::source_location Src = std::source_location::current();

	private:
		TaskCompletionSource(std::shared_ptr<Awaiter<T>> InAwaiter)
			: MyAwaiter(std::move(InAwaiter))
		{
		}

	public:
		TaskCompletionSource() = default;
		TaskCompletionSource(const TaskCompletionSource&) = default;
		TaskCompletionSource(TaskCompletionSource&&) = default;

		bool IsValid() const
		{
			return (bool)MyAwaiter;
		}

		void SetResult(std::source_location InSrc = std::source_location::current()) const
		{
			SetResultImpl();
			Src = InSrc;
		}

		template<class U>
		void SetResult(U&& Result, std::source_location InSrc = std::source_location::current()) const
		{
			SetResultImpl(std::forward<U>(Result));
			Src = InSrc;
		}

		template<class TException>
		void SetException(const TException& ExceptionObj) const
		{
			SetException(std::make_exception_ptr(ExceptionObj));
		}

		void SetException(std::exception_ptr ExceptionPtr) const
		{
			Xassert(IsValid(), TEXT("MyAwaiter is null."));
			MyAwaiter->SetException(std::move(ExceptionPtr));
		}

		void SetCanceled() const
		{
			Xassert(IsValid(), TEXT("MyAwaiter is null."));
			MyAwaiter->Cancel();
		}

		Task<T> GetTask() const
		{
			return Task<T>(MyAwaiter);
		}

		TaskCompletionSource& operator =(const TaskCompletionSource&) = default;
		TaskCompletionSource& operator =(TaskCompletionSource&&) = default;

		template<class U = T>
		static TaskCompletionSource<U> Create(CancellationToken sToken = {})
		{
			return TaskCompletionSource<U>(std::make_shared<Awaiter<U>>(sToken));
		}

	private:
		template<class... U>
		void SetResultImpl(U&&... Result) const
		{
			Xassert(IsValid(), TEXT("Awaiter is null."));
			MyAwaiter->SetResult(std::forward<U>(Result)...);
		}

		void Xassert(bool bAssert, String InMessage) const
		{
			if (!bAssert)
			{
				throw InvalidOperationException(InMessage);
			}
		}
	};
}