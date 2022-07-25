// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "AwaiterBase.h"
#include "Exceptions/InvalidOperationException.h"
#include "Exceptions/TaskCanceledException.h"
#include "Exceptions/TaskAbortedException.h"
#include "Misc/VoidableOptional.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include "co_push.h"
#include <coroutine>
#include <queue>
#include <stop_token>
#include <memory>

template<class T>
class Awaiter : public AwaiterBase
{
	template<class U>
	friend class Awaiter;

	Spinlock _lock;
	SpinlockConditionVariable _future;

	VoidableOptional<T> _promise;
	ETaskStatus _status;
	std::exception_ptr _exception;

	std::vector<std::function<void(std::shared_ptr<AwaiterBase>)>> _thens;
	std::vector<std::unique_ptr<std::stop_callback<std::function<void()>>>> _stop_tokens;
	std::vector<std::function<bool()>> _validators;

public:
	Awaiter(std::stop_token sToken = {}, ETaskStatus initialStatus = ETaskStatus::Running)
		: _status(initialStatus)
	{
		if (sToken.stop_possible())
		{
			this->_Add_cancellation_token(nullptr, sToken);
		}
	}

	Awaiter(const Awaiter&) = delete;

	decltype(auto) await_resume()
	{
		return GetResult();
	}

	virtual ETaskStatus GetStatus() const noexcept override
	{
		return _status;
	}

	virtual void ContinueWith(std::function<void(std::shared_ptr<AwaiterBase>)> proc) override
	{
		auto lock = std::unique_lock(_lock);
		if (IsCompleted())
		{
			lock.unlock();
			proc(shared_from_this());
		}
		else
		{
			_thens.emplace_back(std::move(proc));
		}
	}

	virtual std::exception_ptr GetException() override
	{
		auto lock = std::unique_lock<Spinlock>(_lock, Spinlock::Readonly);
		return _exception;
	}

	T GetResult()
	{
		Wait();
		if (_exception)
		{
			std::rethrow_exception(_exception);
		}
		return _promise.GetValue();
	}

	virtual void Wait() noexcept override
	{
		if (IsCompleted())
		{
			return;
		}

		auto lock = std::unique_lock<Spinlock>(_lock, Spinlock::Readonly);
		_future.Wait(lock, [this] { return IsCompleted(); });
	}

	virtual bool WaitFor(const TimeSpan& timeout) noexcept override
	{
		if (IsCompleted())
		{
			return true;
		}

		auto lock = std::unique_lock<Spinlock>(_lock, Spinlock::Readonly);
		return _future.WaitFor(lock, timeout, [this] { return IsCompleted(); });
	}

	virtual bool WaitUntil(const DateTime& timeout) noexcept override
	{
		return WaitFor(timeout - DateTime::Now());
	}

	virtual void Cancel() override
	{
		auto lock = std::unique_lock(_lock);
		if (!IsCompleted())
		{
			_exception = std::make_exception_ptr(TaskCanceledException());
			_status = ETaskStatus::Canceled;

			auto thens = std::move(_thens);
			_future.NotifyAll();
			lock.unlock();
			this->_Invoke_thens(std::move(thens));
		}
	}

	virtual bool SetException(std::exception_ptr ptr) override
	{
		auto lock = std::unique_lock(_lock);
		if (this->IsCancellationRequested())
		{
			lock.unlock();
			Cancel();
			return false;
		}

		if (!IsCompleted())
		{
			_exception = ptr;
			_status = ETaskStatus::Faulted;

			auto thens = std::move(_thens);
			_future.NotifyAll();
			lock.unlock();
			this->_Invoke_thens(std::move(thens));
			return true;
		}
		else
		{
			throw InvalidOperationException(TEXT("Task already completed."));
		}
	}

	virtual suspend_and_destroy_if AddCancellationToken(std::stop_token sToken) override
	{
		if (!sToken.stop_possible())
		{
			return IsCancellationRequested();
		}

		if (sToken.stop_requested())
		{
			Cancel();
			return true;
		}

		auto lock = std::unique_lock(_lock);
		if (this->IsCancellationRequested())
		{
			return true;
		}

		if (IsCompleted())
		{
			return false;
		}

		this->_Add_cancellation_token(std::move(lock), std::move(sToken));
		return IsCancellationRequested();
	}

	virtual suspend_and_destroy_if AddConditionVariable(std::function<bool()> condition) override
	{
		if (condition == nullptr)
		{
			return IsCancellationRequested();
		}

		if (!condition())
		{
			Cancel();
			return true;
		}
		
		auto lock = std::unique_lock(_lock);
		if (this->IsCancellationRequested())
		{
			return true;
		}

		if (IsCompleted())
		{
			return false;
		}

		_validators.emplace_back(condition);
		return IsCancellationRequested();
	}

	virtual bool IsCancellationRequested() const noexcept override
	{
		if (IsCanceled())
		{
			return true;
		}

		for (auto& val : _validators)
		{
			if (!val())
			{
				return true;
			}
		}

		return false;
	}

	inline bool IsCompleted() const noexcept
	{
		ETaskStatus status = GetStatus();
		return status == ETaskStatus::RanToCompletion
			|| status == ETaskStatus::Faulted
			|| status == ETaskStatus::Canceled;
	}

	inline bool IsCompletedSuccessfully() const noexcept
	{
		return _status == ETaskStatus::RanToCompletion;
	}

	inline bool IsCanceled() const noexcept
	{
		return _status == ETaskStatus::Canceled;
	}

	inline bool IsFaulted() const noexcept
	{
		return _status == ETaskStatus::Faulted;
	}

	template<class... U>
	void SetResult(U&&... result) requires
		(std::same_as<T, void> && sizeof...(U) == 0) ||
		(!std::same_as<T, void> && std::constructible_from<T, U...>)
	{
		if (this->IsCancellationRequested())
		{
			this->Cancel();
			return;
		}

		auto lock = std::unique_lock(_lock);
		if (!IsCompleted())
		{
			_promise.Emplace(std::forward<U>(result)...);
			_status = ETaskStatus::RanToCompletion;

			std::vector thens = std::move(_thens);
			_future.NotifyAll();
			lock.unlock();
			this->_Invoke_thens(std::move(thens));
		}
		else
		{
			throw InvalidOperationException(TEXT("Task already completed."));
		}
	}

private:
	template<class _Unilock>
	inline void _Add_cancellation_token(_Unilock&& lock, std::stop_token sToken)
	{
		auto& ptr = _stop_tokens.emplace_back();

		if constexpr (!std::same_as<_Unilock, std::nullptr_t>)
		{
			lock.unlock();
		}

		ptr = std::make_unique<std::stop_callback<std::function<void()>>>(
			sToken,
			[this] { this->Cancel(); }
		);
	}

	void _Invoke_thens(std::vector<std::function<void(std::shared_ptr<AwaiterBase>)>> thens)
	{
		for (auto& invoke : thens)
		{
			this->_Invoke(invoke);
		}
	}

	void _Invoke(std::function<void(std::shared_ptr<AwaiterBase>)>& body)
	{
		body(shared_from_this());
	}
};