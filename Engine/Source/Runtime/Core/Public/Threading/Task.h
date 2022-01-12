// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Awaiter.h"
#include "AwaitableSharedPointer.h"
#include <coroutine>
#include <memory>

template<class T = void>
class Task
{
	template<class U>
	friend class Task;

	Task(const Task&) = delete;
	Task& operator =(const Task&) = delete;

private:
	using MyAwaiter = Threading::Tasks::Awaiter<T>;

public:
	template<class U, class _Void = void>
	struct PromiseBase
	{
		std::shared_ptr<MyAwaiter> Awaiter = std::make_shared<MyAwaiter>();

		void return_value(U ReturnValue)
		{
			Awaiter->SetValue(std::move(ReturnValue));
		}
	};

	template<class _Void>
	struct PromiseBase<void, _Void>
	{
		std::shared_ptr<MyAwaiter> Awaiter = std::make_shared<MyAwaiter>();

		void return_void()
		{
			Awaiter->SetValue();
		}
	};

	struct promise_type : public PromiseBase<T>
	{
		promise_type()
		{
		}

		Task get_return_object()
		{
			return Task(*this);
		}

		void unhandled_exception()
		{
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
		auto await_transform(AwaitableTask&& ATask) requires requires
		{
			{ ATask.GetAwaiter() };
		}
		{
			return Threading::Tasks::AwaitableSharedPointer(ATask.GetAwaiter());
		}
	};

private:
	using CoroutineHandle = std::coroutine_handle<promise_type>;

private:
	std::optional<CoroutineHandle> Coroutine;
	std::shared_ptr<MyAwaiter> Awaiter;

private:
	Task(promise_type& Awaitable)
		: Coroutine(CoroutineHandle::from_promise(Awaitable))
		, Awaiter(Awaitable.Awaiter)
	{
	}

public:
	Task()
	{
	}

	Task(Task&& Rhs)
		: Coroutine(std::move(Rhs.Coroutine))
		, Awaiter(std::move(Rhs.Awaiter))
	{
	}

	Task(std::shared_ptr<MyAwaiter> Awaiter)
		: Awaiter(std::move(Awaiter))
	{
	}

	template<class TTask>
	Task(TTask&& OtherTask) requires requires
	{
		{ OtherTask.GetAwaiter() } -> std::same_as<std::shared_ptr<MyAwaiter>>;
	}
		: Awaiter(OtherTask.GetAwaiter())
	{
	}

	bool IsValid() const
	{
		return (bool)Awaiter;
	}

	void Wait()
	{
		Awaiter->Wait();
	}

	T GetValue()
	{
		return Awaiter->GetValue();
	}

	std::shared_ptr<MyAwaiter> GetAwaiter() const
	{
		return Awaiter;
	}

	Task& operator =(Task&& Rhs)
	{
		Coroutine = std::move(Rhs.Coroutine);
		Awaiter = std::move(Rhs.Awaiter);
		return *this;
	}

private:
	template<class _Fn>
	static auto FunctionBodyReturnTypeTraits(int, _Fn&& Body) -> decltype(Body(std::declval<T>()));
	template<class _Fn>
	static auto FunctionBodyReturnTypeTraits(short, _Fn&& Body) -> decltype(Body());

	template<class _Fn>
	using FunctionReturnType = decltype(FunctionBodyReturnTypeTraits(0, std::declval<_Fn>()));

public:
	template<class _Fn>
	auto Then(_Fn&& Body) -> Task<FunctionReturnType<_Fn>>
	{
		using ReturnType = FunctionReturnType<_Fn>;
		check(IsValid());

		std::shared_ptr NewAwaiter = std::make_shared<typename Task<ReturnType>::MyAwaiter>();
		NewAwaiter->SetRunner(Awaiter->GetRunner());

		if constexpr (std::same_as<T, void>)
		{
			Awaiter->Then([Body = std::move(Body), NewAwaiter]() -> void
			{
				if constexpr (std::same_as<ReturnType, void>)
				{
					Body();
					NewAwaiter->SetValue();
				}
				else
				{
					ReturnType ReturnValue = Body();
					NewAwaiter->SetValue(std::move(ReturnValue));
				}
			});
		}
		else
		{
			Awaiter->Then([Body = std::move(Body), NewAwaiter](T Result) -> void
			{
				if constexpr (std::same_as<ReturnType, void>)
				{
					Body(Result);
					NewAwaiter->SetValue();
				}
				else
				{
					ReturnType ReturnValue = Body(Result);
					NewAwaiter->SetValue(std::move(ReturnValue));
				}
			});
		}

		return Task<ReturnType>(std::move(NewAwaiter));
	}
};