// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/Action.h"
#include "System/Func.h"
#include "Threading/Tasks/co_push.h"
#include "Threading/Tasks/co_cancel.h"
#include "Threading/Tasks/suspend_and_destroy_if.h"
#include <memory>
#include <stop_token>
#include <vector>

namespace details
{
	template<class TTask>
	struct AwaiterTrait
	{
		using Type = typename TTask::Awaiter_t;
	};

	template<>
	struct AwaiterTrait<void>
	{
		using Type = void;
	};

	template<class T>
	class WrapSharedAwaiter
	{
		std::shared_ptr<T> Awaiter;

	public:
		inline WrapSharedAwaiter(std::shared_ptr<T> Awaiter) noexcept
			: Awaiter(std::move(Awaiter))
		{
		}

		inline operator std::shared_ptr<T>() const & noexcept
		{
			return Awaiter;
		}

		inline operator std::shared_ptr<T>() const && noexcept
		{
			return std::move(Awaiter);
		}

		inline bool await_ready() const noexcept(noexcept(Awaiter->await_ready()))
		{
			return Awaiter->await_ready();
		}

		template<class TCoroutineHandle>
		inline void await_suspend(TCoroutineHandle&& Coro) const noexcept(noexcept(Awaiter->await_suspend(std::forward<TCoroutineHandle>(Coro))))
		{
			Awaiter->await_suspend(std::forward<TCoroutineHandle>(Coro));
		}

		inline decltype(auto) await_resume() const noexcept(noexcept(Awaiter->await_resume()))
		{
			return Awaiter->await_resume();
		}

		inline T* operator ->() const noexcept
		{
			return Awaiter.get();
		}
	};

	template<class T, class TTask>
	class promise_type_base
	{
	public:
		using Awaiter_t = typename AwaiterTrait<TTask>::Type;
		using Task_t = TTask;

	private:
		std::shared_ptr<Awaiter_t> Awaiter;

	protected:
		template<class... TArgs>
		constexpr promise_type_base(TArgs&&... InArgs) noexcept
		{
			if constexpr (!std::same_as<Awaiter_t, void>)
			{
				Awaiter = std::make_shared<Awaiter_t>(std::forward<TArgs>(InArgs)...);
			}
		}

	public:
		Awaiter_t* GetAwaiter() const noexcept requires
			(!std::same_as<Awaiter_t, void>)
		{
			return Awaiter.get();
		}

		constexpr TTask get_return_object() noexcept requires (!std::same_as<TTask, void>)
		{
			return TTask(Awaiter);
		}

		constexpr void get_return_object() noexcept requires std::same_as<TTask, void>
		{
		}

		constexpr void unhandled_exception()
		{
			if constexpr (std::same_as<Awaiter_t, void>)
			{
				std::rethrow_exception(std::current_exception());
			}
			else
			{
				if (Awaiter)
				{
					Awaiter->SetException(std::current_exception());
				}
			}
		}

		constexpr auto initial_suspend() noexcept
		{
			return std::suspend_never();
		}

		constexpr auto final_suspend() noexcept
		{
			return std::suspend_never();
		}

		template<class TAwaitableTask>
		constexpr decltype(auto) await_transform(TAwaitableTask&& InTask)
		{
			return WrapSharedAwaiter(InTask.GetAwaiter());
		}

		template<class TOp>
		auto yield_value(co_push_t<TOp>&& InPush)
		{
			if constexpr (std::same_as<TOp, std::stop_token>)
			{
				return GetAwaiter()->AddCancellationToken(std::move(InPush)._Consume());
			}
			else if constexpr (std::same_as<TOp, std::function<bool()>>)
			{
				return GetAwaiter()->AddConditionVariable(std::move(InPush)._Consume());
			}
			else
			{
				return std::suspend_never();
			}
		}

		constexpr suspend_and_destroy_if yield_value(co_cancel_t&&)
		{
			return true;
		}
	};
}

template<class T, class TTask>
class promise_type : public details::promise_type_base<T, TTask>
{
public:
	promise_type()
	{
	}

	template<class U>
	void return_value(U&& InValue) requires std::constructible_from<T, U>
	{
		this->GetAwaiter()->SetResult(T(std::forward<U>(InValue)));
	}

	void return_value(const T& InValue)
	{
		this->GetAwaiter()->SetResult(InValue);
	}

	void return_value(T&& InValue)
	{
		this->GetAwaiter()->SetResult(std::move(InValue));
	}
};

template<class TTask> requires
	requires { std::declval<TTask>().GetAwaiter()->SetResult(); }
class promise_type<void, TTask> : public details::promise_type_base<void, TTask>
{
public:
	promise_type()
	{
	}

	void return_void()
	{
		this->GetAwaiter()->SetResult();
	}
};

class async_void_promise_type : public details::promise_type_base<void, void>
{
	bool bCancellationRequested = false;
	std::vector<std::unique_ptr<std::stop_callback<Action<>>>> StopCallbacks;
	std::vector<Func<bool>> Validators;

public:
	inline async_void_promise_type() noexcept
	{
	}

	inline void return_void() noexcept
	{
	}

	inline async_void_promise_type* GetAwaiter() noexcept
	{
		return this;
	}

	inline void Cancel() noexcept
	{
		bCancellationRequested = true;
	}

	inline bool IsCancellationRequested() const
	{
		if (bCancellationRequested)
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

	suspend_and_destroy_if AddCancellationToken(std::stop_token sToken)
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

		if (this->IsCancellationRequested())
		{
			return true;
		}

		StopCallbacks.emplace_back(std::make_unique<std::stop_callback<Action<>>>(
			sToken,
			[this] { this->Cancel(); }
		));

		return IsCancellationRequested();
	}

	suspend_and_destroy_if AddConditionVariable(Func<bool> ConditionBody)
	{
		if (ConditionBody.IsBound() == false)
		{
			return IsCancellationRequested();
		}

		if (!ConditionBody())
		{
			Cancel();
			return true;
		}

		if (this->IsCancellationRequested())
		{
			return true;
		}

		Validators.emplace_back(ConditionBody);
		return IsCancellationRequested();
	}
};

template<class TOwningClass, class... TArgs>
struct std::coroutine_traits<void, TOwningClass, TArgs...>
{
	using promise_type = ::async_void_promise_type;
};

template<class... TArgs>
struct std::coroutine_traits<void, TArgs...>
{
	using promise_type = ::async_void_promise_type;
};