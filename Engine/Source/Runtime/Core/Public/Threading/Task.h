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
		Else([this]()
		{
			Coroutine->destroy();
		});
	}

public:
	Task()
	{
	}

	Task(const Task& Rhs)
		: Coroutine(Rhs.Coroutine)
		, Awaiter(Rhs.Awaiter)
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
		Task(std::forward<TTask>(OtherTask), 0);
	} : Task(std::forward<TTask>(OtherTask), 0)
	{
	}

private:
	template<class TTask>
	Task(TTask&& OtherTask, int) requires requires
	{
		{ OtherTask.GetAwaiter() } -> std::same_as<std::shared_ptr<MyAwaiter>>;
	} : Awaiter(OtherTask.GetAwaiter())
	{
	}

	template<class TTask>
	Task(TTask&& OtherTask, short) requires std::same_as<T, void> && requires
	{
		{ AsVoidTask(std::forward<TTask>(OtherTask)) };
	} : Task(AsVoidTask(std::forward<TTask>(OtherTask)))
	{
	}

public:
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

	Threading::Tasks::EStatus GetStatus() const
	{
		return Awaiter->GetStatus();
	}

	std::shared_ptr<MyAwaiter> GetAwaiter() const
	{
		return Awaiter;
	}

	bool Cancel()
	{
		return Awaiter->Cancel();
	}

	Task& operator =(const Task& Rhs)
	{
		Coroutine = Rhs.Coroutine;
		Awaiter = Rhs.Awaiter;
		return *this;
	}

	Task& operator =(Task&& Rhs)
	{
		Coroutine = std::move(Rhs.Coroutine);
		Awaiter = std::move(Rhs.Awaiter);
		return *this;
	}

	auto operator <=>(const Task& Rhs) const
	{
		return Awaiter <=> Rhs.Awaiter;
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

	template<class TAnyTask>
	static Task<> AsVoidTask(TAnyTask&& AnyTask)
	{
		co_await AnyTask;
	}

public:
	template<class _Fn>
	auto Then(_Fn&& Body) -> Task<FunctionReturnType<_Fn>>
	{
		using ReturnType = FunctionReturnType<_Fn>;
		check(IsValid());

		std::shared_ptr NewAwaiter = std::make_shared<typename Task<ReturnType>::MyAwaiter>();

		if constexpr (std::same_as<T, void>)
		{
			Awaiter->Then([Body = Body, NewAwaiter]() -> void
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
			Awaiter->Then([Body = Body, NewAwaiter](T Result) -> void
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
		Awaiter->Else(std::forward<_Fn>(Body));
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
			if constexpr (std::same_as<decltype(It.GetValue()), void>)
			{
				It.Then([Awaiter]()
				{
					Awaiter->IncrementSetValue();
				});
			}
			else
			{
				It.Then([Awaiter](auto)
				{
					Awaiter->IncrementSetValue();
				});
			}

			It.Else([Awaiter]()
			{
				Awaiter->Cancel();
			});
		}

		return Task<>(Awaiter);
	}

	template<class... TTaskParams>
	static Task<> WhenAll(TTaskParams&&... Tasks) requires requires
	{
		{ std::vector{ Task<>(std::forward<TTaskParams>(Tasks))... } };
	}
	{
		return WhenAll(std::vector{ Task<>(std::forward<TTaskParams>(Tasks))... });
	}
};