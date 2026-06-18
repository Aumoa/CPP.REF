// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/SharedTask.h"
#include <memory>
#include <coroutine>
#include <concepts>

namespace Ayla
{
	template<class T, class Task>
	class PromiseTypeBase
	{
		std::shared_ptr<SharedTask<T>> m_Task;

	protected:
		PromiseTypeBase()
		{
			m_Task = std::make_shared<SharedTask<T>>();
		}

		SharedTask<T>* GetTask() noexcept
		{
			return m_Task.get();
		}

	public:
		PromiseTypeBase(const PromiseTypeBase&) = delete;

		Task get_return_object() noexcept
		{
			return Task(m_Task);
		}

		void unhandled_exception() noexcept
		{
			bool b = m_Task->TrySetException(std::current_exception());
			check(b);
		}

		auto initial_suspend() noexcept
		{
			m_Task->TransitToRunning();
			return std::suspend_never();
		}

		constexpr auto final_suspend() const noexcept
		{
			return std::suspend_never();
		}

		template<class AwaitableTask>
		constexpr decltype(auto) await_transform(AwaitableTask&& task) noexcept
		{
			return task.GetAwaiter();
		}
	};

	template<class T, class Task>
	class PromiseType : public PromiseTypeBase<T, Task>
	{
	public:
		PromiseType()
		{
		}

		template<class U>
		void return_value(U&& value) requires std::constructible_from<T, U>
		{
			this->GetTask()->SetResult(std::forward<U>(value));
		}
	};

	template<class Task>
	class PromiseType<void, Task> : public PromiseTypeBase<void, Task>
	{
	public:
		PromiseType()
		{
		}

		void return_void()
		{
			this->GetTask()->SetResult();
		}
	};
}
