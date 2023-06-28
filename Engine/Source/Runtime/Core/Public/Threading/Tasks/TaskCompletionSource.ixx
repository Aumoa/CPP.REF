// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Threading:TaskCompletionSource;

export import Core.Std;
export import :Awaiter;
export import :Task;

export template<class T = void>
class TaskCompletionSource
{
	template<class>
	friend class TaskCompletionSource;

	std::shared_ptr<Awaiter<T>> Awaiter;
	std::source_location Src = std::source_location::current();

private:
	TaskCompletionSource(std::shared_ptr<::Awaiter<T>> InAwaiter)
		: Awaiter(std::move(InAwaiter))
	{
	}

public:
	TaskCompletionSource() = default;
	TaskCompletionSource(const TaskCompletionSource&) = default;
	TaskCompletionSource(TaskCompletionSource&&) = default;

	bool IsValid() const
	{
		return (bool)Awaiter;
	}

	void SetResult(std::source_location InSrc = std::source_location::current())
	{
		SetResultImpl();
		Src = InSrc;
	}

	template<class U>
	void SetResult(U&& Result, std::source_location InSrc = std::source_location::current())
	{
		SetResultImpl(std::forward<U>(Result));
		Src = InSrc;
	}

	template<class TException>
	void SetException(const TException& ExceptionObj)
	{
		SetException(std::make_exception_ptr(ExceptionObj));
	}

	void SetException(std::exception_ptr ExceptionPtr)
	{
		Xassert(IsValid(), TEXT("Awaiter is null."));
		Awaiter->SetException(std::move(ExceptionPtr));
	}

	void SetCanceled()
	{
		Xassert(IsValid(), TEXT("Awaiter is null."));
		Awaiter->Cancel();
	}

	Task<T> GetTask()
	{
		return Task<T>(Awaiter);
	}

	TaskCompletionSource& operator =(const TaskCompletionSource&) = default;
	TaskCompletionSource& operator =(TaskCompletionSource&&) = default;

	template<class U = T>
	static TaskCompletionSource<U> Create(std::stop_token sToken = {})
	{
		return TaskCompletionSource<U>(std::make_shared<::Awaiter<U>>(sToken));
	}

private:
	template<class... U>
	void SetResultImpl(U&&... Result)
	{
		Xassert(IsValid(), TEXT("Awaiter is null."));
		Awaiter->SetResult(std::forward<U>(Result)...);
	}

	void Xassert(bool bAssert, String InMessage)
	{
		if (!bAssert)
		{
			throw InvalidOperationException(InMessage);
		}
	}
};