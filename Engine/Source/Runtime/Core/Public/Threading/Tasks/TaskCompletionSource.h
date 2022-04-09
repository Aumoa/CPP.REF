// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Awaiter.h"
#include "Task.h"
#include <source_location>

namespace libty::inline Core::inline Threading::inline Tasks
{
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

		void SetResult(const std::source_location& source = std::source_location::current())
		{
			SetResultImpl(source);
		}

		template<class U>
		void SetResult(U&& result, const std::source_location& source = std::source_location::current())
		{
			SetResultImpl(source, std::forward<U>(result));
		}

		void SetException(std::exception_ptr ptr, const std::source_location& source = std::source_location::current())
		{
			Xassert(IsValid(), "Awaiter is null.");
			_awaiter->SetException(std::move(ptr), source);
		}

		void SetCanceled(const std::source_location& source = std::source_location::current())
		{
			Xassert(IsValid(), "Awaiter is null.");
			_awaiter->Cancel(source);
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
		template<class... U>
		void SetResultImpl(const std::source_location& source, U&&... result)
		{
			Xassert(IsValid(), "Awaiter is null.");
			_awaiter->SetResult(std::forward<U>(result)...);
		}

		void Xassert(bool x, std::string_view message)
		{
			if (!x)
			{
				throw InvalidOperationException(message);
			}
		}
	};
}