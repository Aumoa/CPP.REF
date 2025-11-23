// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/ThreadPool.h"
#include "Threading/Tasks/TaskAwaiter.h"
#include "Threading/Tasks/PromiseType.h"
#include "Threading/Tasks/YieldAwaitable.h"
#include <memory>

namespace Ayla
{
	template<class T = void>
	class [[nodiscard]] Task
	{
		template<class U>
		friend class Task;

	public:
		using promise_type = PromiseType<T, Task<T>>;
		using ValueType = T;

	private:
		std::shared_ptr<SharedTask<T>> m_Task;

	public:
		Task() = default;
		Task(const Task&) = default;

		template<class U>
		explicit Task(std::shared_ptr<U> task) requires
			std::constructible_from<Task, std::shared_ptr<U>, int>
			: Task(task, 0)
		{
		}

		Task(std::shared_ptr<SharedTask<T>> task, int)
			: m_Task(std::move(task))
		{
		}

		explicit Task(std::shared_ptr<SharedTask<T>> task, short)
			: m_Task(std::move(task))
		{
		}

		explicit Task(const Task<>& task) requires (!std::same_as<T, void>)
			: Task(std::static_pointer_cast<SharedTask<T>>(task.GetShared()))
		{
		}

		Task(Task&& task)
			: m_Task(std::move(task.m_Task))
		{
		}

		inline bool IsValid() const noexcept
		{
			return (bool)m_Task;
		}

		inline std::shared_ptr<SharedTask<T>> GetShared() const
		{
			return m_Task;
		}

		inline TaskAwaiter<T> GetAwaiter() const noexcept
		{
			check(IsValid());
			return TaskAwaiter<T>(m_Task);
		}

		inline TaskStatus GetStatus() const noexcept
		{
			check(IsValid());
			return m_Task->GetStatus();
		}

		inline std::exception_ptr GetException() const noexcept
		{
			check(IsValid());
			return m_Task->GetException();
		}

		template<class TBody>
		auto ContinueWith(TBody&& continuationBody, std::stop_token cancellationToken = {}) const -> Task<std::invoke_result_t<TBody, Task>>
		{
			using U = std::invoke_result_t<TBody, Task>;
			check(IsValid());
			std::shared_ptr otherTask = std::make_shared<SharedTask<U>>(cancellationToken);
			otherTask->TransitToRunning();
			m_Task->ContinueWith([continuationBody = std::forward<TBody>(continuationBody), selfTask = m_Task, otherTask]() mutable
			{
				try
				{
					if constexpr (std::same_as<U, void>)
					{
						continuationBody(Task(selfTask));
						otherTask->SetResult();
					}
					else
					{
						auto r = continuationBody(Task(selfTask));
						otherTask->SetResult(std::move(r));
					}
				}
				catch (...)
				{
					otherTask->SetException(std::current_exception());
				}
			});

			return Task<U>(std::move(otherTask));
		}

		Task<bool> SuppressCancellationThrow() requires std::same_as<T, void>;
		Task<std::optional<T>> SuppressCancellationThrow() requires (!std::same_as<T, void>);

		inline void Wait() const noexcept
		{
			check(IsValid());
			m_Task->Wait();
		}

		inline Task<T> WaitAsync(std::stop_token cancellationToken) const noexcept
		{
			check(IsValid());
			return ContinueWith([](Task<T> task)
			{
				return task.GetResult();
			}, cancellationToken);
		}

		inline T GetResult() const
		{
			check(IsValid());
			if constexpr (std::same_as<T, void>)
			{
				m_Task->Wait();
				auto e = m_Task->GetException();
				if (e)
				{
					std::rethrow_exception(e);
				}
			}
			else
			{
				return m_Task->GetResult();
			}
		}

		inline bool IsCompleted() const noexcept
		{
			return m_Task && m_Task->IsCompleted();
		}

		inline bool IsCompletedSuccessfully() const noexcept
		{
			return m_Task && m_Task->GetStatus() == TaskStatus::RanToCompletion;
		}

		inline bool IsCanceled() const noexcept
		{
			return m_Task && m_Task->GetStatus() == TaskStatus::Canceled;
		}

		inline bool IsFaulted() const noexcept
		{
			return m_Task && m_Task->GetStatus() == TaskStatus::Faulted;
		}

		Task& operator =(const Task&) = default;
		Task& operator =(Task&&) = default;

		template<class U>
		explicit operator Task<U>() const requires
			std::same_as<T, void>
		{
			return Task<U>(m_Task);
		}

		template<class U>
		operator Task<U>() const requires
			std::same_as<U, void> &&
			(!std::same_as<T, void>)
		{
			return Task<U>(m_Task);
		}

		auto operator <=>(const Task&) const = default;
		bool operator ==(const Task&) const = default;

	public:
		template<class TBody>
		static auto Run(TBody&& continuationBody, std::stop_token cancellationToken = {}) -> Task<std::invoke_result_t<TBody>>
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");

			using U = std::invoke_result_t<TBody>;
			std::shared_ptr task = std::make_shared<SharedTask<U>>(cancellationToken);
			task->TransitToRunning();

			ThreadPool::QueueUserWorkItem([task, continuationBody = std::forward<TBody>(continuationBody)]() mutable
			{
				try
				{
					if constexpr (std::same_as<U, void>)
					{
						continuationBody();
						task->SetResult();
					}
					else
					{
						U result = continuationBody();
						task->SetResult(std::move(result));
					}
				}
				catch (...)
				{
					bool b = task->TrySetException(std::current_exception());
					check(b);
				}
			});

			return Task<U>(std::move(task));
		}

		static YieldAwaitable Yield()
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Yield instead.");
			return YieldAwaitable();
		}

		static Task<> Delay(std::chrono::milliseconds delay, std::stop_token cancellationToken = {})
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");

			std::shared_ptr task = std::make_shared<SharedTask<void>>(cancellationToken);
			task->TransitToRunning();
			ThreadPool::QueueDelayedUserWorkItem(delay, [task]() mutable
			{
				task->SetResult();
			});

			return Task<>(std::move(task));
		}

		static Task<> CompletedTask()
		{
			static_assert(std::same_as<T, void>, "Use Task<>::CompletedTask instead.");

			static thread_local std::shared_ptr sTask = []
			{
				auto ptr = std::make_shared<SharedTask<void>>();
				ptr->TransitToRunning();
				ptr->SetResult();
				return ptr;
			}();

			return Task<>(sTask);
		}

		template<class U> requires (!std::same_as<U, void>)
			static Task<U> FromResult(U value)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::FromResult<U> instead.");

			auto task = std::make_shared<SharedTask<U>>();
			task->TransitToRunning();
			task->SetResult(std::move(value));
			return Task<U>(task);
		}
	};

	template<class T>
	Task<bool> Task<T>::SuppressCancellationThrow() requires std::same_as<T, void>
	{
		return ContinueWith([](Task<void> task)
		{
			if (task.GetStatus() == TaskStatus::Canceled)
			{
				return false;
			}
			else
			{
				task.GetResult();
				return true;
			}
		});
	}

	template<class T>
	Task<std::optional<T>> Task<T>::SuppressCancellationThrow() requires (!std::same_as<T, void>)
	{
		return ContinueWith([](Task<T> task) -> std::optional<T>
		{
			if (task.GetStatus() == TaskStatus::Canceled)
			{
				return std::nullopt;
			}
			else
			{
				return task.GetResult();
			}
		});
	}
}