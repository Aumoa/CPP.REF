// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IAwaiter.h"
#include "ITask.h"
#include "Misc/ContainsRef.h"
#include "co_push.h"
#include "co_cancel.h"
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

	template<class T>
	class WrapSharedAwaiter
	{
		std::shared_ptr<T> _awaiter;

	public:
		inline WrapSharedAwaiter(std::shared_ptr<T> awaiter) noexcept
			: _awaiter(std::move(awaiter))
		{
		}

		inline operator std::shared_ptr<T>() const & noexcept
		{
			return _awaiter;
		}

		inline operator std::shared_ptr<T>() const && noexcept
		{
			return std::move(_awaiter);
		}

		inline bool await_ready() const noexcept(noexcept(_awaiter->await_ready()))
		{
			return _awaiter->await_ready();
		}

		template<class TCoroutineHandle>
		inline void await_suspend(TCoroutineHandle&& coroutine) const noexcept(noexcept(_awaiter->await_suspend(std::forward<TCoroutineHandle>(coroutine))))
		{
			_awaiter->await_suspend(std::forward<TCoroutineHandle>(coroutine));
		}

		inline decltype(auto) await_resume() const noexcept(noexcept(_awaiter->await_resume()))
		{
			return _awaiter->await_resume();
		}

		inline T* operator ->() const noexcept
		{
			return _awaiter.get();
		}
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
		Awaiter_t* GetAwaiter() const noexcept requires
			(!std::same_as<Awaiter_t, void>)
		{
			return _awaiter.get();
		}

		constexpr TTask get_return_object() noexcept requires (!std::same_as<TTask, void>)
		{
			return TTask(_awaiter);
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
			ITask<std::remove_const_t<std::remove_reference_t<TAwaitableTask>>>
		{
			return WrapSharedAwaiter(task.GetAwaiter());
		}

		template<class TOp>
		auto yield_value(co_push<TOp>&& push)
		{
			if constexpr (std::same_as<TOp, std::stop_token>)
			{
				return GetAwaiter()->AddCancellationToken(std::move(push)._Consume());
			}
			else if constexpr (std::same_as<TOp, std::function<bool()>>)
			{
				return GetAwaiter()->AddConditionVariable(std::move(push)._Consume());
			}
			else
			{
				return std::suspend_never();
			}
		}

		suspend_and_destroy_if yield_value(co_cancel_t&&)
		{
			return true;
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
	bool _cancellationRequested = false;
	std::vector<std::unique_ptr<std::stop_callback<std::function<void()>>>> _stop_tokens;
	std::vector<std::function<bool()>> _validators;

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
		_cancellationRequested = true;
	}

	inline bool IsCancellationRequested() const
	{
		if (_cancellationRequested)
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

		_stop_tokens.emplace_back(std::make_unique<std::stop_callback<std::function<void()>>>(
			sToken,
			[this] { this->Cancel(); }
		));

		return IsCancellationRequested();
	}

	suspend_and_destroy_if AddConditionVariable(std::function<bool()> condition)
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

		if (this->IsCancellationRequested())
		{
			return true;
		}

		_validators.emplace_back(condition);
		return IsCancellationRequested();
	}
};

template<class TOwningClass, class... TArgs>
struct std::coroutine_traits<void, TOwningClass, TArgs...>
{
	using promise_type = ::async_void_promise_type;
	static_assert(!ContainsRef<TArgs...>, "async void function cannot include reference parameter.");
};