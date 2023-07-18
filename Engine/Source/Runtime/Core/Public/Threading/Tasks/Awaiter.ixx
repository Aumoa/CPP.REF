// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Threading:Awaiter;

export import Core.Std;
export import Core.System;
export import :AwaiterBase;
export import :co_push;
export import :Spinlock;
export import :SpinlockConditionVariable;
export import :TaskCanceledException;

template<class T>
class Awaiter : public AwaiterBase
{
	template<class U>
	friend class Awaiter;
	using TCallback = std::stop_callback<std::function<void()>>;

	Spinlock Lock;
	SpinlockConditionVariable Future;

	VoidableOptional<T> Promise;
	ETaskStatus Status;
	std::exception_ptr ExceptionPtr;

	std::vector<std::function<void(std::shared_ptr<AwaiterBase>)>> Thens;
	std::vector<std::unique_ptr<TCallback>> StopCallbacks;
	std::vector<std::function<bool()>> Validators;

public:
	Awaiter(std::stop_token sToken = {}, ETaskStatus InitialStatus = ETaskStatus::Running)
		: Status(InitialStatus)
	{
		if (sToken.stop_possible())
		{
			this->Add_cancellation_token(nullptr, sToken);
		}
	}

	Awaiter(const Awaiter&) = delete;

	decltype(auto) await_resume()
	{
		return GetResult();
	}

	virtual ETaskStatus GetStatus() const noexcept override
	{
		return Status;
	}

	virtual void ContinueWith(std::function<void(std::shared_ptr<AwaiterBase>)> ContinuationBody) override
	{
		auto ScopedLock = std::unique_lock(Lock);
		if (IsCompleted())
		{
			ScopedLock.unlock();
			ContinuationBody(shared_from_this());
		}
		else
		{
			Thens.emplace_back(std::move(ContinuationBody));
		}
	}

	virtual std::exception_ptr GetException() override
	{
		auto ScopedLock = std::unique_lock(Lock.AsReadonly());
		return ExceptionPtr;
	}

	T GetResult()
	{
		Wait();
		if (ExceptionPtr)
		{
			std::rethrow_exception(ExceptionPtr);
		}
		return Promise.GetValue();
	}

	virtual void Wait() noexcept override
	{
		if (IsCompleted())
		{
			return;
		}

		auto ScopedLock = std::unique_lock(Lock.AsReadonly());
		Future.Wait(ScopedLock, [this] { return IsCompleted(); });
	}

	virtual bool WaitFor(const TimeSpan& Timeout) noexcept override
	{
		if (IsCompleted())
		{
			return true;
		}

		auto ScopedLock = std::unique_lock(Lock.AsReadonly());
		return Future.WaitFor(ScopedLock, Timeout, [this] { return IsCompleted(); });
	}

	virtual bool WaitUntil(const DateTime& Timeout) noexcept override
	{
		return WaitFor(Timeout - DateTime::Now());
	}

	virtual void Cancel() override
	{
		auto ScopedLock = std::unique_lock(Lock);
		if (!IsCompleted())
		{
			ExceptionPtr = std::make_exception_ptr(TaskCanceledException());
			Status = ETaskStatus::Canceled;

			auto LocalThens = std::move(Thens);
			Future.NotifyAll();
			ScopedLock.unlock();
			this->Invoke_thens(std::move(LocalThens));
		}
	}

	virtual bool SetException(std::exception_ptr InExceptionPtr) override
	{
		auto ScopedLock = std::unique_lock(Lock);
		if (this->IsCancellationRequested())
		{
			ScopedLock.unlock();
			Cancel();
			return false;
		}

		if (!IsCompleted())
		{
			ExceptionPtr = InExceptionPtr;
			Status = ETaskStatus::Faulted;

			auto LocalThens = std::move(Thens);
			Future.NotifyAll();
			ScopedLock.unlock();
			this->Invoke_thens(std::move(LocalThens));
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

		auto ScopedLock = std::unique_lock(Lock);
		if (this->IsCancellationRequested())
		{
			return true;
		}

		if (IsCompleted())
		{
			return false;
		}

		this->Add_cancellation_token(std::move(ScopedLock), std::move(sToken));
		return IsCancellationRequested();
	}

	virtual suspend_and_destroy_if AddConditionVariable(std::function<bool()> ConditionBody) override
	{
		if (ConditionBody == nullptr)
		{
			return IsCancellationRequested();
		}

		if (!ConditionBody())
		{
			Cancel();
			return true;
		}
		
		auto ScopedLock = std::unique_lock(Lock);
		if (this->IsCancellationRequested())
		{
			return true;
		}

		if (IsCompleted())
		{
			return false;
		}

		Validators.emplace_back(ConditionBody);
		return IsCancellationRequested();
	}

	virtual void AddStopCallback(std::stop_token sToken, std::function<void()> CallbackBody) override
	{
		auto ScopedLock = std::unique_lock(Lock);
		StopCallbacks.emplace_back(std::make_unique<TCallback>(std::move(sToken), std::move(CallbackBody)));
	}

	virtual bool IsCancellationRequested() const noexcept override
	{
		if (IsCanceled())
		{
			return true;
		}

		for (auto& Validator : Validators)
		{
			if (!Validator())
			{
				return true;
			}
		}

		return false;
	}

	inline bool IsCompleted() const noexcept
	{
		ETaskStatus LocalStatus = GetStatus();
		return LocalStatus == ETaskStatus::RanToCompletion
			|| LocalStatus == ETaskStatus::Faulted
			|| LocalStatus == ETaskStatus::Canceled;
	}

	inline bool IsCompletedSuccessfully() const noexcept
	{
		return Status == ETaskStatus::RanToCompletion;
	}

	inline bool IsCanceled() const noexcept
	{
		return Status == ETaskStatus::Canceled;
	}

	inline bool IsFaulted() const noexcept
	{
		return Status == ETaskStatus::Faulted;
	}

	template<class... U>
	void SetResult(U&&... Result) requires
		(std::same_as<T, void> && sizeof...(U) == 0) ||
		(!std::same_as<T, void> && std::constructible_from<T, U...>)
	{
		if (this->IsCancellationRequested())
		{
			this->Cancel();
			return;
		}

		auto ScopedLock = std::unique_lock(Lock);
		if (!IsCompleted())
		{
			Promise.Emplace(std::forward<U>(Result)...);
			Status = ETaskStatus::RanToCompletion;

			std::vector LocalThens = std::move(Thens);
			Future.NotifyAll();
			ScopedLock.unlock();
			this->Invoke_thens(std::move(LocalThens));
		}
		else
		{
			throw InvalidOperationException(TEXT("Task already completed."));
		}
	}

private:
	template<class ScopedLock_t>
	inline void Add_cancellation_token(ScopedLock_t&& ScopedLock, std::stop_token sToken)
	{
		auto& StopCallback = StopCallbacks.emplace_back();

		if constexpr (!std::same_as<ScopedLock_t, std::nullptr_t>)
		{
			ScopedLock.unlock();
		}

		StopCallback = std::make_unique<TCallback>(
			sToken,
			[this] { this->Cancel(); }
		);
	}

	void Invoke_thens(std::vector<std::function<void(std::shared_ptr<AwaiterBase>)>> InThens)
	{
		for (auto& Invoke : InThens)
		{
			this->Invoke(Invoke);
		}
	}

	void Invoke(std::function<void(std::shared_ptr<AwaiterBase>)>& InBody)
	{
		InBody(shared_from_this());
	}
};