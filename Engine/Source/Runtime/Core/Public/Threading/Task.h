// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Awaiter.h"
#include <coroutine>
#include <memory>
#include <chrono>

class ThreadGroup;

#ifndef TASK_FALLBACK_DEFERRED_DEFAULT
#define TASK_FALLBACK_DEFERRED_DEFAULT 0
#endif

namespace Threading::Tasks
{
	template<class T>
	class TaskSource
	{
	protected:
		using SourceAwaiter = Awaiter<T>;
		std::shared_ptr<SourceAwaiter> Awaiter;

	public:
		TaskSource()
		{
		}

		TaskSource(std::shared_ptr<SourceAwaiter> Awaiter)
			: Awaiter(std::move(Awaiter))
		{
		}
	};

	template<>
	class CORE_API TaskSource<void>
	{
	protected:
		using SourceAwaiter = IAwaitable;
		std::shared_ptr<IAwaitable> Awaiter;

	public:
		static std::optional<bool> bFallbackToDeferred;

	public:
		TaskSource();
		TaskSource(std::shared_ptr<SourceAwaiter> Awaiter);

		static void FlushDeferred();

	protected:
		static void Run(std::function<void()> Body);
		static void Deferred(std::function<void()> Body);
		static void Delay(std::chrono::milliseconds Timeout, std::function<void()> Body);
		static std::shared_ptr<Tasks::Awaiter<void>> CompletedAwaiter();
	};
}

template<class T = void>
class Task : public Threading::Tasks::TaskSource<T>
{
	template<class U>
	friend class Task;

private:
	using MyAwaiter = Threading::Tasks::Awaiter<T>;
	using Super = Threading::Tasks::TaskSource<T>;
	using SourceAwaiter = Super::SourceAwaiter;
	template<class U>
	using TaskSource = Threading::Tasks::TaskSource<U>;

	class AwaitableSharedPointer
	{
		std::shared_ptr<SourceAwaiter> Ptr;

	public:
		AwaitableSharedPointer(std::shared_ptr<SourceAwaiter> Ptr) : Ptr(std::move(Ptr))
		{
		}

		bool await_ready()
		{
			return Ptr->await_ready();
		}

		template<class TCoroutine>
		void await_suspend(TCoroutine Coroutine) const noexcept
		{
			Ptr->await_suspend(std::move(Coroutine));
		}

		T await_resume() const noexcept
		{
			return Ptr->await_resume();
		}

		SourceAwaiter* operator ->() const
		{
			return Ptr.get();
		}
	};

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
			this->Awaiter->SetExceptionPtr(std::current_exception());
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
			using NakedTask = std::remove_const_t<std::remove_reference_t<decltype(ATask)>>;
			return typename NakedTask::AwaitableSharedPointer(ATask.GetAwaiter());
		}
	};

private:
	using CoroutineHandle = std::coroutine_handle<promise_type>;

private:
	std::optional<CoroutineHandle> Coroutine;

private:
	Task(promise_type& Awaitable)
		: Super(Awaitable.Awaiter)
		, Coroutine(CoroutineHandle::from_promise(Awaitable))
	{
		Else([this]()
		{
			Coroutine->destroy();
		});
	}

public:
	Task() : Super()
	{
	}

	Task(const Task& Rhs)
		: Super(Rhs.Awaiter)
		, Coroutine(Rhs.Coroutine)
	{
	}

	Task(Task&& Rhs)
		: Super(Rhs.Awaiter)
		, Coroutine(std::move(Rhs.Coroutine))
	{
	}

	Task(std::shared_ptr<MyAwaiter> Awaiter)
		: Super(Awaiter)
	{
	}

	template<class TTask>
	Task(TTask&& OtherTask) requires requires
	{
		{ Super(OtherTask.GetAwaiter()) };
	} : Super(OtherTask.GetAwaiter())
	{
	}

public:
	bool IsValid() const
	{
		return (bool)Super::Awaiter;
	}

	void Wait()
	{
		Super::Awaiter->Wait();
	}

	T GetValue()
	{
		return Super::Awaiter->GetValue();
	}

	Threading::Tasks::EStatus GetStatus() const
	{
		return Super::Awaiter->GetStatus();
	}

	std::shared_ptr<SourceAwaiter> GetAwaiter() const
	{
		return Super::Awaiter;
	}

	bool Cancel()
	{
		return Super::Awaiter->Cancel();
	}

	Task& operator =(const Task& Rhs)
	{
		Coroutine = Rhs.Coroutine;
		Super::Awaiter = Rhs.Awaiter;
		return *this;
	}

	Task& operator =(Task&& Rhs)
	{
		Coroutine = std::move(Rhs.Coroutine);
		Super::Awaiter = std::move(Rhs.Awaiter);
		return *this;
	}

	auto operator <=>(const Task& Rhs) const
	{
		return Super::Awaiter <=> Rhs.Awaiter;
	}

	auto operator < (const Task& Rhs) const { return operator <=>(Rhs) < 0; }
	auto operator > (const Task& Rhs) const { return operator <=>(Rhs) > 0; }
	auto operator <=(const Task& Rhs) const { return operator <=>(Rhs) <= 0; }
	auto operator >=(const Task& Rhs) const { return operator <=>(Rhs) >= 0; }
	auto operator ==(const Task& Rhs) const { return operator <=>(Rhs) == 0; }
	auto operator !=(const Task& Rhs) const { return operator <=>(Rhs) != 0; }

public:
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
		checkf(IsValid(), L"Task not valid.");

		std::shared_ptr NewAwaiter = std::make_shared<typename Task<ReturnType>::MyAwaiter>();

		if constexpr (std::same_as<T, void>)
		{
			Super::Awaiter->Then([Body = Body, NewAwaiter]() mutable -> void
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
			Super::Awaiter->Then([Body = Body, NewAwaiter](T Result) mutable -> void
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

	template<class _Fn>
	void Else(_Fn&& Body)
	{
		Super::Awaiter->Else(std::forward<_Fn>(Body));
	}

public:
	template<class TEnuemrable>
	static Task<> WhenAll(TEnuemrable&& Enumerable) requires requires
	{
		{ std::begin(Enumerable) } -> std::forward_iterator;
		{ std::end(Enumerable) } -> std::forward_iterator;
		{ std::size(Enumerable) } -> std::same_as<size_t>;
	}
	{
		class WhenAllAwaiter : public Threading::Tasks::Awaiter<void>
		{
			const size_t Desired;
			std::atomic<size_t> Counter;

		public:
			WhenAllAwaiter(size_t Desired) : Desired(Desired)
			{
			}

			void IncrementSetValue()
			{
				size_t MyCounter = ++Counter;
				if (MyCounter == Desired)
				{
					SetValue();
				}
			}
		};

		std::shared_ptr Awaiter = std::make_shared<WhenAllAwaiter>(std::size(Enumerable));

		for (auto& It : Enumerable)
		{
			Task<> UniversalTask = It;
			UniversalTask.Then([Awaiter]()
			{
				Awaiter->IncrementSetValue();
			});

			UniversalTask.Else([Awaiter]()
			{
				Awaiter->Cancel();
			});
		}

		return Task<>(Awaiter);
	}

	template<class... TTaskParams>
	static Task<> WhenAll(TTaskParams&&... Tasks) requires requires
	{
		std::initializer_list{ Task<>(std::declval<TTaskParams>())... };
	}
	{
		return WhenAll(std::vector{ Task<>(std::forward<TTaskParams>(Tasks))... });
	}
	
	template<bool bConstFallbackToDeferred = (bool)TASK_FALLBACK_DEFERRED_DEFAULT, class _Fn>
	static auto Run(_Fn&& Body) requires std::same_as<T, void>
	{
		using ReturnType = FunctionReturnType<_Fn>;
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		TaskSource<void>::Run([Awaiter, Body = std::move(Body)]() mutable -> void
		{
			if constexpr (std::same_as<ReturnType, void>)
			{
				Body();

				if (TaskSource<void>::bFallbackToDeferred.value_or(bConstFallbackToDeferred))
				{
					TaskSource<void>::Deferred([Awaiter = std::move(Awaiter)]()
					{
						Awaiter->SetValue();
					});
				}
				else
				{
					Awaiter->SetValue();
				}
			}
			else
			{
				ReturnType ReturnValue = Body();

				if (TaskSource<void>::bFallbackToDeferred.value_or(bConstFallbackToDeferred))
				{
					TaskSource<void>::Deferred([ReturnValue = std::move(ReturnValue), Awaiter = std::move(Awaiter)]() mutable
					{
						Awaiter->SetValue(std::move(ReturnValue));
					});
				}
				else
				{
					Awaiter->SetValue(std::move(ReturnValue));
				}
			}

			Awaiter.reset();
		});

		return Awaiter;
	}

	static Task<> Yield() requires std::same_as<T, void>
	{
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		TaskSource<void>::Run([Awaiter]() mutable -> void
		{
			Awaiter->SetValue();
		});

		return Awaiter;
	}

	template<class _Fn>
	static auto Deferred(_Fn&& Body) requires std::same_as<T, void>
	{
		using ReturnType = FunctionReturnType<_Fn>;
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		TaskSource<void>::Deferred([Awaiter, Body = std::move(Body)]() mutable -> void
		{
			if constexpr (std::same_as<ReturnType, void>)
			{
				Body();
				Awaiter->SetValue();
			}
			else
			{
				ReturnType ReturnValue = Body();
				Awaiter->SetValue(std::move(ReturnValue));
			}
		});

		return Awaiter;
	}

	static Task<> Deferred() requires std::same_as<T, void>
	{
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		TaskSource<void>::Deferred([Awaiter]() mutable -> void
		{
			Awaiter->SetValue();
		});

		return Awaiter;
	}

	static Task<> Delay(uint32 Timeout) requires std::same_as<T, void> { return Delay(std::chrono::milliseconds(Timeout)); }
	static Task<> Delay(std::chrono::milliseconds Timeout) requires std::same_as<T, void>
	{
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		TaskSource<void>::Delay(Timeout, [Awaiter]() mutable -> void
		{
			Awaiter->SetValue();
		});

		return Awaiter;
	}

	template<class T, class U>
	static void SetManual(Task<T>& Task, U&& Value)
	{
		Task.Awaiter->SetValue(std::forward<U>(Value));
	}

	template<bool bConstFallbackToDeferred = (bool)TASK_FALLBACK_DEFERRED_DEFAULT, class _Fn>
	static Task<> WaitUntil(_Fn&& ConditionBody) requires requires
	{
		{ std::declval<_Fn>()() } -> std::convertible_to<bool>;
	}
	{
		auto Body = std::forward<_Fn>(ConditionBody);
		while (!Body())
		{
			co_await Yield();
		}

		if (TaskSource<void>::bFallbackToDeferred.value_or(bConstFallbackToDeferred))
		{
			co_await Deferred();
		}
	}

	static Task<> CompletedTask()
	{
		return TaskSource<void>::CompletedAwaiter();
	}
};