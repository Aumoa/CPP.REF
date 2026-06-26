// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/ThreadPool.h"
#include "Threading/SynchronizationContext.h"
#include "Threading/Tasks/TaskStatus.h"
#include "Threading/Tasks/TaskCreationOptions.h"
#include "InvalidOperationException.h"
#include "TaskCanceledException.h"
#include "MoveOnlyFunction.h"
#include <mutex>
#include <condition_variable>
#include <exception>
#include <vector>
#include <optional>
#include <stop_token>
#include <memory>

namespace Ayla
{
	class TaskScheduler;
	class TaskFactory;

	template<class T = void, int kCheck = 0>
	class SharedTask
	{
	};

	template<>
	class SharedTask<void, 0>
	{
		friend class TaskScheduler;
		friend class TaskFactory;

	public:
		template<class TBody>
		using function_t = MoveOnlyFunction<TBody>;

	protected:
		mutable std::mutex m_Mutex;
		mutable std::condition_variable m_Future;

		TaskStatus m_Status = TaskStatus::Created;
		std::exception_ptr m_ExceptionPtr;
		TaskCreationOptions m_Options = TaskCreationOptions::None;
		SharedTask<>::function_t<void()> m_Scheduled;

		std::stop_token m_StoppedToken;
		std::source_location m_StoppedLocation;

		std::vector<std::tuple<function_t<void()>, std::weak_ptr<SynchronizationContext>>> m_Continuations;
		std::optional<std::stop_callback<function_t<void()>>> m_Cancellation;

	public:
		SharedTask(std::stop_token cancellationToken = {})
		{
			if (cancellationToken.stop_possible())
			{
				m_Cancellation.emplace(cancellationToken, [this]()
				{
					this->TryCancel();
				});
			}
		}

		SharedTask(const SharedTask&) = delete;

		~SharedTask() noexcept
		{
		}

		void TransitToRunning() noexcept
		{
			std::unique_lock lock(m_Mutex);
			check(m_Status == TaskStatus::Created);
			m_Status = TaskStatus::Running;
		}

		inline TaskStatus GetStatus() const noexcept
		{
			return m_Status;
		}

		inline TaskCreationOptions GetOptions() const noexcept
		{
			return m_Options;
		}

		void ContinueWith(function_t<void()> continuation, bool continueOnCapturedContext) noexcept
		{
			std::weak_ptr<SynchronizationContext> context = continueOnCapturedContext ? SynchronizationContext::GetCurrent() : nullptr;
			std::unique_lock lock(m_Mutex);
			if (IsCompleted())
			{
				lock.unlock();
				this->Invoke(std::move(continuation), context);
			}
			else
			{
				m_Continuations.emplace_back(std::move(continuation), context);
			}
		}

		std::exception_ptr GetException() const noexcept
		{
			std::unique_lock lock(m_Mutex);
			return m_ExceptionPtr;
		}

		void Wait() const noexcept
		{
			if (IsCompleted())
			{
				return;
			}

			std::unique_lock lock(m_Mutex);
			m_Future.wait(lock, [this] { return IsCompleted(); });
		}

		void GetResult() const
		{
			Wait();

			// No additional locking is necessary because Wait has already ensured the task is completed.
			if (m_ExceptionPtr)
			{
				std::rethrow_exception(m_ExceptionPtr);
			}
			else if (m_Status == TaskStatus::Canceled)
			{
				throw TaskCanceledException(m_StoppedToken, m_StoppedLocation);
			}
		}

		void SetResult()
		{
			std::unique_lock lock(m_Mutex);
			if (IsCompleted())
			{
				throw InvalidOperationException(TEXT("Task already completed."));
			}

			check(m_Status == TaskStatus::Running);
			m_Status = TaskStatus::RanToCompletion;

			UnlockAndInvokeContinuations(lock);
		}

		bool TrySetResult()
		{
			std::unique_lock lock(m_Mutex);
			if (IsCompleted())
			{
				return false;
			}

			check(m_Status == TaskStatus::Running);
			m_Status = TaskStatus::RanToCompletion;

			UnlockAndInvokeContinuations(lock);
			return true;
		}

		bool TryCancel(std::stop_token stoppingToken = {}, std::source_location src = std::source_location::current()) noexcept
		{
			std::unique_lock lock(m_Mutex);
			if (IsCompleted())
			{
				return false;
			}

			check(m_Status == TaskStatus::Running || m_Status == TaskStatus::Created);
			m_Status = TaskStatus::Canceled;
			
			m_StoppedToken = stoppingToken;
			m_StoppedLocation = src;

			UnlockAndInvokeContinuations(lock);
			return true;
		}

		bool TrySetException(std::exception_ptr except) noexcept
		{
			std::unique_lock lock(m_Mutex);
			if (IsCompleted())
			{
				return false;
			}

			try
			{
				std::rethrow_exception(except);
			}
			catch (const OperationCanceledException&)
			{
				check(m_Status == TaskStatus::Running || m_Status == TaskStatus::Created);
				m_Status = TaskStatus::Canceled;
				m_ExceptionPtr = std::current_exception();
			}
			catch (...)
			{
				check(m_Status == TaskStatus::Running);
				m_Status = TaskStatus::Faulted;
				m_ExceptionPtr = std::current_exception();
			}

			UnlockAndInvokeContinuations(lock);
			return true;
		}

		bool IsCompleted() const noexcept
		{
			auto status = GetStatus();
			return status == TaskStatus::RanToCompletion || status == TaskStatus::Faulted || status == TaskStatus::Canceled;
		}

	protected:
		void UnlockAndInvokeContinuations(std::unique_lock<std::mutex>& lock)
		{
			auto cc = std::move(m_Continuations);
			m_Future.notify_all();
			lock.unlock();

			for (auto& c : cc)
			{
				this->Invoke(std::move(std::get<0>(c)), std::get<1>(c));
			}
		}

	private:
		void Invoke(function_t<void()> continuation, std::weak_ptr<SynchronizationContext> context) noexcept
		{
			auto context_ptr = context.lock();
			if (context_ptr)
			{
				context_ptr->Post(std::move(continuation));
				return;
			}

			ThreadPool::QueueUserWorkItem(std::move(continuation));
		}
	};

	template<class T> requires (!std::is_same_v<T, void>)
	class SharedTask<T, 0> : public SharedTask<void, 0>
	{
		mutable std::optional<T> m_Promise;

	public:
		SharedTask(std::stop_token cancellationToken = {}) : SharedTask<void, 0>(std::move(cancellationToken))
		{
		}

		SharedTask(const SharedTask&) = delete;

		~SharedTask() noexcept
		{
		}

		T& GetResult() const
		{
			Wait();

			// No additional locking is necessary because Wait has already ensured the task is completed.
			if (m_ExceptionPtr)
			{
				std::rethrow_exception(m_ExceptionPtr);
			}

			return m_Promise.value();
		}

		template<class... U>
		void SetResult(U&&... args) requires std::constructible_from<T, U...>
		{
			std::unique_lock lock(m_Mutex);
			if (IsCompleted())
			{
				throw InvalidOperationException(TEXT("Task already completed."));
			}

			check(m_Status == TaskStatus::Running);
			m_Promise.emplace(std::forward<U>(args)...);
			m_Status = TaskStatus::RanToCompletion;

			UnlockAndInvokeContinuations(lock);
		}

		template<class... U>
		bool TrySetResult(U&&... args) requires std::constructible_from<T, U...>
		{
			std::unique_lock lock(m_Mutex);
			if (IsCompleted())
			{
				return false;
			}

			check(m_Status == TaskStatus::Running);
			m_Promise.emplace(std::forward<U>(args)...);
			m_Status = TaskStatus::RanToCompletion;

			UnlockAndInvokeContinuations(lock);
			return true;
		}
	};
}
