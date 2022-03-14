// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Awaiter.h"
#include "CoreConcepts.h"
#include <coroutine>
#include <memory>
#include <chrono>

class ThreadGroup;

template<class T>
class Task;

namespace libty::Threading::Tasks::Impl
{
	template<class T>
	struct SourceAwaiter
	{
		using type = Awaiter<T>;
	};

	template<>
	struct SourceAwaiter<void>
	{
		using type = IAwaiter;
	};

	template<class T>
	class WrapSharedAwaiter
	{
		T _awaiter;

	public:
		WrapSharedAwaiter(T awaiter) : _awaiter(std::move(awaiter))
		{
		}

		auto await_ready()
		{
			return _awaiter->await_ready();
		}

		template<class TCoroutine>
		auto await_suspend(TCoroutine&& coroutine)
		{
			return _awaiter->await_suspend(std::forward<TCoroutine>(coroutine));
		}

		auto await_resume()
		{
			return _awaiter->await_resume();
		}
	};

	template<class T>
	class promise_type_base
	{
	protected:
		std::shared_ptr<Awaiter<T>> _awaiter = std::make_shared<Awaiter<T>>();
		promise_type_base() = default;

	public:
		std::shared_ptr<Awaiter<T>> GetAwaiter() const
		{
			return _awaiter;
		}

		Task<T> get_return_object()
		{
			return Task<T>(_awaiter);
		}

		void unhandled_exception()
		{
			_awaiter->SetException(std::current_exception());
		}

		auto initial_suspend() noexcept
		{
			return std::suspend_never();
		}

		auto final_suspend() noexcept
		{
			return std::suspend_never();
		}

		template<class AwaitableTask>
		auto await_transform(AwaitableTask&& task) requires requires
		{
			{ task.GetAwaiter() };
		}
		{
			auto awaiter = task.GetAwaiter();
			return WrapSharedAwaiter(std::move(awaiter));
		}
	};

	template<class T>
	class promise_type : public promise_type_base<T>
	{
	public:
		promise_type() = default;

		template<class U>
		void return_value(U&& value) requires requires
		{
			{ std::declval<Awaiter<T>>().SetResult(std::declval<U>()) };
		}
		{
			this->_awaiter->SetResult(std::forward<U>(value));
		}
	};

	template<>
	class promise_type<void> : public promise_type_base<void>
	{
	public:
		promise_type() = default;

		void return_void()
		{
			this->_awaiter->SetResult();
		}
	};
}

template<class T = void>
class Task
{
	template<class U>
	friend class Task;

public:
	using promise_type = libty::Threading::Tasks::Impl::promise_type<T>;

protected:
	using MyAwaiter = Awaiter<T>;
	using SourceAwaiter = typename libty::Threading::Tasks::Impl::SourceAwaiter<T>::type;
	using CoroutineHandle = std::coroutine_handle<promise_type>;

private:
	std::shared_ptr<SourceAwaiter> _awaiter;

public:
	Task() = default;
	Task(const Task&) = default;
	Task(Task&&) = default;

	Task(std::shared_ptr<SourceAwaiter> awaiter)
		: _awaiter(std::move(awaiter))
	{
	}

	template<class U>
	Task(std::shared_ptr<Awaiter<U>> awaiter) requires std::derived_from<Awaiter<U>, SourceAwaiter>
		: _awaiter(std::move(awaiter))
	{
	}

	bool IsValid() const
	{
		return (bool)_awaiter;
	}

	std::shared_ptr<Awaiter<T>> GetAwaiter() const
	{
		if constexpr (std::convertible_to<SourceAwaiter, Awaiter<T>>)
		{
			return _awaiter;
		}
		else
		{
			return static_pointer_cast<Awaiter<T>>(_awaiter);
		}
	}

	void Wait()
	{
		ThrowInvalid();
		_awaiter->Wait();
	}

	T GetResult()
	{
		ThrowInvalid();
		return _awaiter->GetResult();
	}

	std::exception_ptr GetException()
	{
		return _awaiter->GetException();
	}

	ETaskStatus GetStatus() const
	{
		return _awaiter->GetStatus();
	}

	bool IsCompleted() const
	{
		return _awaiter->IsCompleted();
	}

	bool IsCompletedSuccessfully() const
	{
		return _awaiter->IsCompletedSuccessfully();
	}

	bool IsCanceled() const
	{
		return _awaiter->IsCanceled();
	}

	Task& operator =(const Task&) = default;
	Task& operator =(Task&&) = default;

	template<class U>
	explicit operator Task<U>() const
	{
		return Task<U>(std::dynamic_pointer_cast<Awaiter<U>>(_awaiter));
	}

	template<class U>
	operator Task<U>() const requires
		std::same_as<U, void> &&
		(!std::same_as<T, void>)
	{
		return Task<U>(_awaiter);
	}

	constexpr auto operator <=>(const Task&) const = default;
	constexpr bool operator ==(const Task&) const = default;

private:
	void ThrowInvalid()
	{
		if (!IsValid())
		{
			throw invalid_operation("Task is null.");
		}
	}

public:
	template<class TBody>
	auto Then(TBody&& body, const std::source_location& source = std::source_location::current()) -> Task<FunctionReturn_t<TBody, Task>>
	{
		using U = FunctionReturn_t<TBody, Task>;

		ThrowInvalid();
		std::shared_ptr uAwaiter = std::make_shared<Awaiter<U>>();

		_awaiter->Then([body = std::forward<TBody>(body), uAwaiter, source = source](Task<> result) mutable
		{
			if constexpr (std::same_as<U, void>)
			{
				body((Task<T>)result);
				uAwaiter->SetResult(source);
			}
			else
			{
				auto r = body((Task<T>)result);
				uAwaiter->SetResult(std::move(r), source);
			}
		});

		return Task<U>(std::move(uAwaiter));
	}

private:
	static void RunImpl(std::function<void()> body)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");
	}

	static void DelayImpl(std::chrono::milliseconds delay, std::function<void()> body)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");
	}

public:
	template<class TBody>
	static auto Run(TBody&& body, const std::source_location& source = std::source_location::current()) -> Task<FunctionReturn_t<TBody>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");
	}

	static auto Yield(const std::source_location& source = std::source_location::current())
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Yield instead.");
	}

	static Task<> Delay(std::chrono::milliseconds delay, const std::source_location& source = std::source_location::current())
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");
	}

	static Task<> CompletedTask()
	{
		static_assert(std::same_as<T, void>, "Use Task<>::CompletedTask instead.");
	}

	template<class U> requires (!std::same_as<U, void>)
	static Task<> CompletedTask(U value, const std::source_location& source = std::source_location::current())
	{
		static_assert(std::same_as<T, void>, "Use Task<>::CompletedTask<U> instead.");
	}
};

template<>
void CORE_API Task<>::RunImpl(std::function<void()> body);

template<>
void CORE_API Task<>::DelayImpl(std::chrono::milliseconds delay, std::function<void()> body);

template<>
template<class TBody>
auto Task<>::Run(TBody&& body, const std::source_location& source) -> Task<FunctionReturn_t<TBody>>
{
	using U = FunctionReturn_t<TBody>;
	std::shared_ptr awaiter = std::make_shared<Awaiter<U>>();
	awaiter->WaitingToRun();

	RunImpl([awaiter, body = std::forward<TBody>(body), source = source]() mutable
	{
		awaiter->Running();

		try
		{
			if constexpr (std::same_as<U, void>)
			{
				body();
				awaiter->SetResult((const std::source_location&)source);
			}
			else
			{
				U result = body();
				awaiter->SetResult(result, source);
			}
		}
		catch (...)
		{
			awaiter->SetException(std::current_exception(), source);
		}
	});

	return Task<U>(std::move(awaiter));
}

template<>
auto Task<>::Yield(const std::source_location& source)
{
	return Run([] {}, source);
}

template<>
Task<> Task<>::Delay(std::chrono::milliseconds delay, const std::source_location& source)
{
	std::shared_ptr awaiter = std::make_shared<Awaiter<void>>();
	awaiter->WaitingToRun();

	DelayImpl(delay, [awaiter, source = source]() mutable
	{
		awaiter->Running();
		awaiter->SetResult((const std::source_location&)source);
	});

	return Task<>(std::move(awaiter));
}

template<>
Task<> Task<>::CompletedTask()
{
	static thread_local std::shared_ptr awaiter = []
	{
		auto ptr = std::make_shared<Awaiter<void>>();
		ptr->SetResult();
		ptr->Freeze();
		return ptr;
	}();

	return Task<>(awaiter);
}

template<>
template<class U> requires (!std::same_as<U, void>)
Task<> Task<>::CompletedTask(U value, const std::source_location& source)
{
	auto ptr = std::make_shared<Awaiter<U>>();
	ptr->SetResult(std::move(value), source);
	return ptr;
}