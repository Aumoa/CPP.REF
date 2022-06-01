// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IAwaiter.h"
#include "ITask.h"
#include "Concepts/ContainsRef.h"
#include <stop_token>
#include <memory>
#include <coroutine>

namespace libty::Details
{
	template<class TTask>
	struct AwaiterTrait
	{
		using Type = typename TTask::Awaiter;
	};

	template<>
	struct AwaiterTrait<void>
	{
		using Type = void;
	};

	template<class T, class TTask>
	class promise_type_base
	{
	public:
		using Awaiter_t = typename AwaiterTrait<TTask>::Type;
		using Task_t = TTask;

	private:
		std::shared_ptr<Awaiter_t> _awaiter;

	protected:
		template<class... TArgs>
		constexpr promise_type_base(TArgs&&... args) noexcept
		{
			if constexpr (!std::same_as<Awaiter_t, void>)
			{
				_awaiter = std::make_shared<Awaiter_t>(std::forward<TArgs>(args)...);
			}
		}

	public:
		template<class TSelf>
		auto GetAwaiter() const noexcept
		{
			return std::ref(_awaiter);
		}

		constexpr TTask get_return_object() noexcept requires !std::same_as<TTask, void>
		{
			return TTask(*this);
		}

		constexpr void get_return_object() noexcept requires std::same_as<TTask, void>
		{
		}

		constexpr void unhandled_exception() noexcept
		{
			if constexpr (std::same_as<Awaiter_t, void>)
			{
				std::rethrow_exception(std::current_exception());
			}
			else
			{
				if (_awaiter)
				{
					_awaiter->SetException(std::current_exception());
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
		constexpr decltype(auto) await_transform(TAwaitableTask&& task) requires
			ITask<TAwaitableTask>
		{
			return task.GetAwaiter();
		}
	};
}

template<class T, class TTask>
class promise_type : public libty::Details::promise_type_base<T, TTask>
{
public:
	promise_type()
	{
	}

	template<class U>
	void return_value(U&& value) requires std::constructible_from<T, U>
	{
		this->GetAwaiter()->SetResult(T(std::forward<U>(value)));
	}

	void return_value(const T& value)
	{
		this->GetAwaiter()->SetResult(value);
	}

	void return_value(T&& value)
	{
		this->GetAwaiter()->SetResult(std::move(value));
	}
};

template<class TTask> requires
	requires { std::declval<TTask>().GetAwaiter()->SetResult(); }
class promise_type<void, TTask> : public libty::Details::promise_type_base<void, TTask>
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

class async_void_promise_type : public libty::Details::promise_type_base<void, void>
{
public:
	async_void_promise_type()
	{
	}

	void return_void()
	{
	}
};

template<class TOwningClass, class... TArgs>
struct std::coroutine_traits<void, TOwningClass, TArgs...>
{
	using promise_type = ::async_void_promise_type;
	static_assert(!ContainsRef<TArgs...>, "async void function cannot include reference parameter.");
};