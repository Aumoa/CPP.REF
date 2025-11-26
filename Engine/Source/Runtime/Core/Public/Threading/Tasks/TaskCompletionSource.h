// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/Task.h"
#include "Threading/Tasks/SharedTask.h"

namespace Ayla
{
	template<class T = void>
	class TaskCompletionSource
	{
		template<class>
		friend class TaskCompletionSource;

		std::shared_ptr<SharedTask<T>> m_Task;

	private:
		TaskCompletionSource(std::shared_ptr<SharedTask<T>> task)
			: m_Task(std::move(task))
		{
		}

	public:
		TaskCompletionSource() = default;
		TaskCompletionSource(const TaskCompletionSource&) = default;
		TaskCompletionSource(TaskCompletionSource&&) = default;

		bool IsValid() const
		{
			return (bool)m_Task;
		}

		void SetResult() const
		{
			SetResultImpl();
		}

		template<class U>
		void SetResult(U&& result) const
		{
			SetResultImpl(std::forward<U>(result));
		}

		template<class TException>
		bool TrySetException(const TException& exceptionObject) const
		{
			return TrySetException(std::make_exception_ptr(exceptionObject));
		}

		bool TrySetException(std::exception_ptr exceptionPtr) const
		{
			Xassert(IsValid(), TEXT("m_Task is null."));
			return m_Task->TrySetException(std::move(exceptionPtr));
		}

		bool TrySetCanceled() const
		{
			Xassert(IsValid(), TEXT("m_Task is null."));
			return m_Task->TryCancel();
		}

		Task<T> GetTask() const
		{
			return Task<T>(m_Task);
		}

		TaskCompletionSource& operator =(const TaskCompletionSource&) = default;
		TaskCompletionSource& operator =(TaskCompletionSource&&) = default;

		template<class U = T>
		static TaskCompletionSource<U> Create(std::stop_token cancellationToken = {})
		{
			return TaskCompletionSource<U>(std::make_shared<SharedTask<U>>(cancellationToken));
		}

	private:
		template<class... U>
		void SetResultImpl(U&&... result) const
		{
			Xassert(IsValid(), TEXT("Awaiter is null."));
			m_Task->SetResult(std::forward<U>(result)...);
		}

		void Xassert(bool condition, String message) const
		{
			if (!condition)
			{
				throw InvalidOperationException(message);
			}
		}
	};
}