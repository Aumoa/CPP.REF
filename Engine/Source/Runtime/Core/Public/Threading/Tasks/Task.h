// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/ThreadPool.h"
#include "Threading/Tasks/TaskStatus.h"
#include "Threading/Tasks/TaskAwaiter.h"
#include "Threading/Tasks/PromiseType.h"
#include "Threading/Tasks/YieldAwaitable.h"
#include "Threading/Tasks/ConfiguredTaskAwaitable.h"
#include "Threading/Tasks/SharedTask.h"
#include "IntegralTypes.h"
#include "VoidableVector.h"
#include "AggregateException.h"
#include "TaskCanceledException.h"
#include "AssertionMacros.h"
#include <memory>
#include <ranges>
#include <concepts>
#include <utility>
#include <algorithm>
#include <exception>
#include <type_traits>
#include <stop_token>
#include <chrono>
#include <vector>
#include <atomic>
#include <tuple>

namespace Ayla
{
	class TaskFactory;

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
			: Task(std::move(task), 0)
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
					otherTask->TrySetException(std::current_exception());
				}
			}, false);

			return Task<U>(std::move(otherTask));
		}

		Task<bool> SuppressCancellationThrow() requires std::same_as<T, void>;
		Task<std::optional<T>> SuppressCancellationThrow() requires (!std::same_as<T, void>);

		ConfiguredTaskAwaitable<T> ConfigureAwait(bool continueOnCapturedContext) const noexcept
		{
			check(IsValid());
			return ConfiguredTaskAwaitable<T>(m_Task, continueOnCapturedContext);
		}

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
		// TaskFactory.h
		static std::shared_ptr<TaskFactory> GetFactory();

		// TaskFactory.h
		template<class TBody>
		static auto Run(TBody&& continuationBody, std::stop_token cancellationToken = {}) -> Task<std::invoke_result_t<TBody>>;

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

		template<std::ranges::input_range IR>
		static auto WhenAll(IR&& tasks) requires std::convertible_to<std::ranges::range_value_t<IR>, Task<>>
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");

			using VT = std::ranges::range_value_t<IR>;
			using V = typename VT::ValueType;

			struct State
			{
				size_t m_SizeResult;
				std::atomic<size_t> m_SizeCompleted;
				VoidableVector<V> m_Results;
				std::vector<std::exception_ptr> m_Exceptions;
				std::shared_ptr<SharedTask<VoidableVector<V>>> m_Task;
			};

			auto state = std::make_shared<State>();
			state->m_Task = std::make_shared<SharedTask<VoidableVector<V>>>();
			state->m_SizeResult = std::ranges::size(tasks);
			state->m_Results.resize(state->m_SizeResult);
			state->m_Exceptions.resize(state->m_SizeResult);
			state->m_Task->TransitToRunning();
			size_t i = 0;
			for (auto& task : tasks)
			{
				std::ignore = task.ContinueWith([state, l = i++](auto t)
				{
					// Collect results or exceptions from each task
					if (t.IsCompletedSuccessfully())
					{
						if constexpr (!std::is_void_v<V>)
						{
							state->m_Results[l] = t.GetResult();
						}
					}
					else if (t.IsCanceled())
					{
						state->m_Exceptions[l] = std::make_exception_ptr(TaskCanceledException());
					}
					else
					{
						state->m_Exceptions[l] = t.GetException();
					}

					// All tasks completed - set final result
					if (++state->m_SizeCompleted == state->m_SizeResult)
					{
						std::vector<std::exception_ptr> innerExceptions;
						for (auto& exception : state->m_Exceptions)
						{
							if (exception)
							{
								innerExceptions.emplace_back(std::move(exception));
							}
						}

						if (!innerExceptions.empty())
						{
							state->m_Task->TrySetException(std::make_exception_ptr(AggregateException(std::move(innerExceptions))));
						}
						else
						{
							state->m_Task->SetResult(std::move(state->m_Results));
						}
					}
				});
			}

			return Task<VoidableVector<V>>(state->m_Task).ContinueWith([state](auto p)
			{
				if constexpr (!std::is_void_v<V>)
				{
					return std::vector<V>(p.GetResult());
				}
			});
		}

		template<class IR>
		static auto WhenAll(IR&& tasks) requires std::convertible_to<std::ranges::range_value_t<IR>, Task<>> && (!std::ranges::sized_range<IR>)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");

			auto v = std::ranges::to<std::vector>(std::forward<IR>(tasks));
			return WhenAll(std::move(v));
		}

		template<class... Tasks>
		static Task<> WhenAll(Tasks&&... tasks) requires (std::convertible_to<Tasks, Task<>> && ...)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");
			
			std::vector<Task<>> taskVector;
			taskVector.reserve(sizeof...(Tasks));
			(taskVector.push_back(std::forward<Tasks>(tasks)), ...);
			
			return WhenAll(std::move(taskVector));
		}

		template<class U, class... Tasks>
		static Task<std::vector<U>> WhenAll(Task<U> first, Tasks&&... rest) 
			requires ((std::same_as<Tasks, Task<U>> && ...) && !std::same_as<U, void>)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");
			
			std::vector<Task<U>> taskVector;
			taskVector.reserve(1 + sizeof...(Tasks));
			taskVector.push_back(std::move(first));
			(taskVector.push_back(std::forward<Tasks>(rest)), ...);
			
			return WhenAll(std::move(taskVector));
		}

		template<std::ranges::input_range IR>
		static auto WhenAny(IR&& tasks) requires std::convertible_to<std::ranges::range_value_t<IR>, Task<>>
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAny instead.");

			using VT = std::ranges::range_value_t<IR>;
			using V = typename VT::ValueType;

			struct State
			{
				std::atomic<bool> m_Completed;
				std::shared_ptr<SharedTask<VT>> m_Task;
				std::vector<VT> m_OriginalTasks;
			};

			auto state = std::make_shared<State>();
			state->m_Task = std::make_shared<SharedTask<VT>>();
			state->m_Completed = false;
			state->m_Task->TransitToRunning();

			// Store original tasks for result
			for (auto& task : tasks)
			{
				state->m_OriginalTasks.push_back(task);
			}

			for (auto& task : state->m_OriginalTasks)
			{
				std::ignore = task.ContinueWith([state](auto t)
				{
					// Only the first completed task sets the result
					bool expected = false;
					if (state->m_Completed.compare_exchange_strong(expected, true))
					{
						// WhenAny returns the completed task itself, not unwrapping exceptions
						state->m_Task->SetResult(t);
					}
				});
			}

			return Task<VT>(state->m_Task);
		}

		template<class IR>
		static auto WhenAny(IR&& tasks) requires std::convertible_to<std::ranges::range_value_t<IR>, Task<>> && (!std::ranges::sized_range<IR>)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAny instead.");

			auto v = std::ranges::to<std::vector>(std::forward<IR>(tasks));
			return WhenAny(std::move(v));
		}

		template<class... Tasks>
		static Task<Task<>> WhenAny(Tasks&&... tasks) requires (std::convertible_to<Tasks, Task<>> && ...)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAny statt.");
			
			std::vector<Task<>> taskVector;
			taskVector.reserve(sizeof...(Tasks));
			(taskVector.push_back(std::forward<Tasks>(tasks)), ...);
			
			return WhenAny(std::move(taskVector));
		}

		template<class U, class... Tasks>
		static Task<Task<U>> WhenAny(Task<U> first, Tasks&&... rest) 
			requires ((std::same_as<Tasks, Task<U>> && ...) && !std::same_as<U, void>)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAny statt.");
			
			std::vector<Task<U>> taskVector;
			taskVector.reserve(1 + sizeof...(Tasks));
			taskVector.push_back(std::move(first));
			(taskVector.push_back(std::forward<Tasks>(rest)), ...);
			
			return WhenAny(std::move(taskVector));
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