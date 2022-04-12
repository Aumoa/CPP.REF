// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IAwaiter.h"
#include "Exceptions/InvalidOperationException.h"
#include "Exceptions/TaskCanceledException.h"
#include "Misc/VoidableOptional.h"
#include <coroutine>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace libty::inline Core
{
	template<class T>
	class Awaiter : public IAwaiter
	{
		std::mutex _lock;
		VoidableOptional<T> _promise;
		std::condition_variable _future;
		ETaskStatus _status = ETaskStatus::Created;
		std::queue<std::function<void(Task<void>)>> _thens;
		std::exception_ptr _exception;
		bool _freezed = false;
		std::source_location _source;

	public:
		Awaiter()
		{
		}

		Awaiter(const Awaiter&) = delete;

		virtual ETaskStatus GetStatus() const override
		{
			return _status;
		}

		virtual bool IsCompleted() const override
		{
			return _status == ETaskStatus::RanToCompletion
				|| _status == ETaskStatus::Faulted
				|| _status == ETaskStatus::Canceled;
		}

		virtual bool IsCompletedSuccessfully() const override
		{
			return _status == ETaskStatus::RanToCompletion;
		}

		virtual bool IsCanceled() const override
		{
			return _status == ETaskStatus::Canceled;
		}

		virtual bool IsFaulted() const override
		{
			return _status == ETaskStatus::Faulted;
		}

		virtual void WaitingToRun() override
		{
			auto lock = std::unique_lock(_lock);
			_status = ETaskStatus::WaitingToRun;
		}

		virtual void Running() override
		{
			auto lock = std::unique_lock(_lock);
			_status = ETaskStatus::Running;
		}

		virtual void Wait() override
		{
			auto lock = std::unique_lock(_lock);
			_future.wait(lock, [this] { return IsCompleted(); });
		}

		virtual void Then(std::function<void(Task<void>)> proc) override
		{
			auto lock = std::unique_lock(_lock);
			if (IsCompleted())
			{
				lock.unlock();
				proc(Task<void>(shared_from_this()));
			}
			else
			{
				_thens.emplace(std::move(proc));
			}
		}

		virtual void Cancel(std::source_location source = std::source_location::current()) override
		{
			auto lock = std::unique_lock(_lock);
			if (_freezed)
			{
				throw InvalidOperationException("Task is freezed.", nullptr, source);
			}
			if (!IsCompleted())
			{
				_status = ETaskStatus::Canceled;
				_exception = std::make_exception_ptr(TaskCanceledException(nullptr, source));
				_source = source;

				_future.notify_all();
				lock.unlock();
				InvokeThens();
			}
			else
			{
				throw InvalidOperationException("Task already completed.", nullptr, source);
			}
		}

		virtual void SetException(std::exception_ptr ptr, std::source_location source = std::source_location::current()) override
		{
			auto lock = std::unique_lock(_lock);
			if (_freezed)
			{
				throw InvalidOperationException("Task is freezed.", nullptr, source);
			}
			if (!IsCompleted())
			{
				_status = ETaskStatus::Faulted;
				_exception = ptr;
				_source = source;

				_future.notify_all();
				lock.unlock();
				InvokeThens();
			}
			else
			{
				throw InvalidOperationException("Task already completed.", nullptr, source);
			}
		}

		virtual std::exception_ptr GetException()
		{
			auto lock = std::unique_lock(_lock);
			return _exception;
		}

		void SetResult(std::source_location source = std::source_location::current()) requires
			std::same_as<T, void>
		{
			SetResultImpl(source);
		}

		template<class U>
		void SetResult(U&& result, std::source_location source = std::source_location::current()) requires
			(!std::same_as<T, void>) &&
			(!std::same_as<std::remove_const_t<std::remove_reference_t<U>>, void>)
		{
			SetResultImpl(source, std::forward<U>(result));
		}

		template<class U = T>
		void GetResult() requires
			std::same_as<U, void> &&
			std::same_as<U, T>
		{
			Wait();
			if (_exception)
			{
				std::rethrow_exception(_exception);
			}
		}

		template<class U = T>
		const U& GetResult() requires
			(!std::same_as<U, void>) &&
			std::same_as<U, T>
		{
			Wait();
			if (_exception)
			{
				std::rethrow_exception(_exception);
			}
			else
			{
				return _promise.GetValue();
			}
		}

		std::source_location GetSourceCode() const
		{
			return _source;
		}

		void Freeze()
		{
			auto lock = std::unique_lock(_lock);
			_freezed = true;
		}

		decltype(auto) await_resume()
		{
			return GetResult();
		}

	private:
		template<class... U>
		void SetResultImpl(std::source_location source, U&&... result)
		{
			auto lock = std::unique_lock(_lock);
			if (_freezed)
			{
				throw InvalidOperationException("Task is freezed.", nullptr, source);
			}
			if (!IsCompleted())
			{
				_status = ETaskStatus::RanToCompletion;
				_promise.Emplace(std::forward<U>(result)...);
				_source = source;

				_future.notify_all();
				lock.unlock();
				InvokeThens();
			}
			else
			{
				throw InvalidOperationException("Task already completed.", nullptr, source);
			}
		}

		void InvokeThens()
		{
			for (; !_thens.empty(); _thens.pop())
			{
				Invoke(_thens.front());
			}
		}

		void Invoke(std::function<void(Task<void>)>& body)
		{
			body(Task<void>(shared_from_this()));
		}
	};
}