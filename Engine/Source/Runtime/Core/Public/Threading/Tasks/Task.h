// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Awaiter.h"
#include "promise_type.h"
#include "Generic/IList.h"
#include "Concepts/CompatibleVariadic.h"
#include "Concepts/GetVariadic.h"
#include <coroutine>
#include <memory>
#include <chrono>
#include <array>

template<class T = void>
class Task
{
	template<class U>
	friend class Task;
	friend class Core;

public:
	using promise_type = ::promise_type<T, Task<T>>;
	using Awaiter = ::Awaiter<T>;
	using ValueType = T;

private:
	std::shared_ptr<AwaiterBase> _awaiter;

public:
	Task() = default;
	Task(const Task&) = default;
	Task(Task&&) = default;

	Task(std::shared_ptr<Awaiter> awaiter)
		: _awaiter(std::move(awaiter))
	{
	}

	explicit Task(std::shared_ptr<AwaiterBase> awaiter)
		: _awaiter(std::dynamic_pointer_cast<Awaiter>(awaiter))
	{
	}

	explicit Task(const Task<>& task) requires !std::same_as<T, void>
		: Task(task.GetAwaiter())
	{
	}

	inline bool IsValid() const noexcept
	{
		return (bool)_awaiter;
	}

	inline std::shared_ptr<Awaiter> GetAwaiter() const noexcept
	{
		return std::static_pointer_cast<Awaiter>(_awaiter);
	}

	inline ETaskStatus GetStatus() const noexcept
	{
		return _awaiter->GetStatus();
	}

	inline std::exception_ptr GetException() const noexcept
	{
		return _awaiter->GetException();
	}
	
	template<class TBody>
	auto ContinueWith(TBody&& body, std::stop_token sToken) const noexcept -> Task<std::invoke_result_t<TBody, Task>>
	{
		using U = std::invoke_result_t<TBody, Task>;
		std::shared_ptr uAwaiter = std::make_shared<::Awaiter<U>>(sToken);
		_awaiter->ContinueWith([body = std::forward<TBody>(body), uAwaiter](std::shared_ptr<AwaiterBase> result) mutable
		{
			try
			{
				if constexpr (std::same_as<U, void>)
				{
					body(Task(result));
					uAwaiter->SetResult();
				}
				else
				{
					auto r = body(Task(result));
					uAwaiter->SetResult(std::move(r));
				}
			}
			catch (const std::exception&)
			{
				uAwaiter->SetException(std::current_exception());
			}
		});

		return Task<U>(std::move(uAwaiter));
	}

	inline void Wait() const noexcept
	{
		_awaiter->Wait();
	}

	inline T GetResult() const noexcept
	{
		return static_cast<Awaiter*>(_awaiter.get())->GetResult();
	}

	inline bool IsCompleted() const noexcept
	{
		return _awaiter->IsCompleted();
	}

	inline bool IsCompletedSuccessfully() const noexcept
	{
		return _awaiter->IsCompletedSuccessfully();
	}

	inline bool IsCanceled() const noexcept
	{
		return _awaiter->IsCanceled();
	}

	inline bool IsFaulted() const noexcept
	{
		return _awaiter->IsFaulted();
	}

	Task& operator =(const Task&) = default;
	Task& operator =(Task&&) = default;

	template<class U>
	explicit operator Task<U>() const requires
		std::same_as<T, void>
	{
		return Task<U>(_awaiter);
	}

	template<class U>
	operator Task<U>() const requires
		std::same_as<U, void> &&
		(!std::same_as<T, void>)
	{
		return Task<U>(_awaiter);
	}

	auto operator <=>(const Task&) const = default;
	bool operator ==(const Task&) const = default;

private:
	static void _Initialize();
	static void _Shutdown();

	static void _Run_thread(std::function<void()> body);
	static void _Delay_thread(std::chrono::milliseconds delay, std::function<void()> body);

public:
	template<class TBody>
	static auto Run(TBody&& body, std::stop_token sToken = {}) -> Task<std::invoke_result_t<TBody>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");

		using U = std::invoke_result_t<TBody>;
		std::shared_ptr uAwaiter = std::make_shared<Awaiter<U>>(sToken);
		uAwaiter->WaitingToRun();

		_Run_thread([uAwaiter, body = std::forward<TBody>(body)]() mutable
		{
			try
			{
				if constexpr (std::same_as<U, void>)
				{
					body();
					uAwaiter->SetResult();
				}
				else
				{
					U result = body();
					uAwaiter->SetResult(result);
				}
			}
			catch (...)
			{
				uAwaiter->SetException(std::current_exception());
			}
		});

		return Task<U>(std::move(uAwaiter));
	}

	static auto Yield()
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Yield instead.");

		std::shared_ptr uAwaiter = std::make_shared<::Awaiter<void>>();
		_Run_thread([uAwaiter]
		{
			uAwaiter->SetResult();
		});
		
		return Task<>(std::move(uAwaiter));
	}

	static Task<> Delay(std::chrono::milliseconds delay, std::stop_token sToken = {})
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");

		std::shared_ptr uAwaiter = std::make_shared<Awaiter<void>>(sToken);
		DelayImpl(delay, [uAwaiter]() mutable
			{
				uAwaiter->SetResult();
			});

		return Task<>(std::move(uAwaiter));
	}

	static Task<> CompletedTask()
	{
		static_assert(std::same_as<T, void>, "Use Task<>::CompletedTask instead.");

		static thread_local std::shared_ptr awaiter = []
		{
			auto ptr = std::make_shared<::Awaiter<void>>();
			ptr->SetResult();
			return ptr;
		}();

		return Task<>(awaiter);
	}

	template<class U> requires (!std::same_as<U, void>)
	static Task<> FromResult(U value)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::FromResult<U> instead.");
		 
		auto ptr = std::make_shared<::Awaiter<U>>();
		ptr->SetResult(std::move(value));
		return ptr;
	}

public:
	template<IList<Task<>> TTasks>
	static Task<> WhenAll(TTasks tasks, std::stop_token sToken = {}) requires
		std::constructible_from<Task<>, EnumerableItem_t<TTasks>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");

		class WhenAllAwaiter : public ::Awaiter<void>
		{
			std::vector<Task<>> _tasks;
			std::atomic<size_t> _counter;
			std::atomic<bool> _bool;

		public:
			WhenAllAwaiter(std::vector<Task<>> tasks, std::stop_token sToken)
				: ::Awaiter<void>(sToken)
				, _tasks(std::move(tasks))
			{
				for (auto& task : tasks)
				{
					task.ContinueWith([self = std::static_pointer_cast<::Awaiter<void>>(this->shared_from_this())](Task<> t)
					{
						if (t.IsCanceled())
						{
							if (bool expected = false; self->_bool.compare_exchange_strong(expected, true))
							{
								self->Cancel();
							}
						}
						else if (auto e = t.GetException())
						{
							if (bool expected = false; self->_bool.compare_exchange_strong(expected, true))
							{
								self->SetException(e);
							}
						}
						else if (auto number = self->_counter; number == self->_tasks.size())
						{
							self->SetResult();
						}
					}, sToken);
				}
			}
		};

		return Task<>(Linq::ToVector(&tasks));
	}
	
	template<class TTasks>
	static auto WhenAll(TTasks tasks, std::stop_token sToken = {})
		-> Task<std::vector<typename EnumerableItem_t<TTasks>::ValueType>> requires
		IEnumerable<TTasks, EnumerableItem_t<TTasks>> &&
		std::constructible_from<Task<>, EnumerableItem_t<TTasks>> &&
		(!std::same_as<typename EnumerableItem_t<TTasks>::ValueType, void>)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");
		using ValueType = typename EnumerableItem_t<ValueType>::ValueType;

		class WhenAllAwaiter : public ::Awaiter<std::vector<ValueType>>
		{
			std::vector<Task<>> _tasks;
			std::atomic<size_t> _counter;
			std::atomic<bool> _bool;
			std::unique_ptr<char[]> _values;

		public:
			WhenAllAwaiter(std::vector<Task<>> tasks, std::stop_token sToken)
				: ::Awaiter<void>(sToken)
				, _tasks(std::move(tasks))
			{
				_values = std::make_unique<char[]>(sizeof(ValueType) * _tasks.size());
				ValueType* values = reinterpret_cast<ValueType*>(_values.get());

				for (size_t i = 0; i < _tasks.size(); ++i)
				{
					_tasks[i].ContinueWith([self = std::static_pointer_cast<::Awaiter<void>>(this->shared_from_this()), values, i](Task<> t)
					{
						if (t.IsCanceled())
						{
							if (bool expected = false; self->_bool.compare_exchange_strong(expected, true))
							{
								self->Cancel();
							}
						}
						else if (auto e = t.GetException())
						{
							if (bool expected = false; self->_bool.compare_exchange_strong(expected, true))
							{
								self->SetException(e);
							}
						}
						else if (auto number = self->_counter; number == self->_tasks.size())
						{
							new(values + i) ValueType(t.GetResult());
							if (number == self->_tasks.size())
							{
								self->SetResult(std::vector(values, values + self->_tasks.size()));
							}
						}
					}, sToken);
				}
			}
		};

		return Task<>(Linq::ToVector(&tasks));
	}

	template<class... TTasks>
	static auto WhenAll(TTasks... tasks) requires
		(std::constructible_from<Task<>, TTasks> && ...)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");

		using BaseVariadic = GetVariadic_t<0, TTasks...>;
		if constexpr (CompatibleVariadic<BaseVariadic, TTasks...>)
		{
			std::array<BaseVariadic, sizeof...(TTasks)> tasksArray{ BaseVariadic(std::move(tasks))... };
			return WhenAll(tasksArray);
		}
		else
		{
			std::array<Task<>, sizeof...(TTasks)> tasksArray{ Task<>(std::move(tasks))... };
			return WhenAll(tasksArray);
		}
	}

	template<class TTasks>
	static Task<EnumerableItem_t<TTasks>> WhenAny(TTasks tasks) requires
		IEnumerable<TTasks, EnumerableItem_t<TTasks>>&&
		std::constructible_from<Task<>, EnumerableItem_t<TTasks>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAny instead.");

		using TTask = EnumerableItem_t<TTasks>;
		class WhenAllAwaiter : public ::Awaiter<TTask>
		{
			std::vector<Task<>> _tasks;
			std::atomic<bool> _bool;

		public:
			WhenAllAwaiter(std::vector<Task<>> tasks, std::stop_token sToken)
				: ::Awaiter<void>(sToken)
				, _tasks(std::move(tasks))
			{
				for (auto& task : tasks)
				{
					task.ContinueWith([self = std::static_pointer_cast<::Awaiter<void>>(this->shared_from_this())](Task<> t)
					{
						if (t.IsCanceled())
						{
							if (bool expected = false; self->_bool.compare_exchange_strong(expected, true))
							{
								self->Cancel();
							}
						}
						else if (auto e = t.GetException())
						{
							if (bool expected = false; self->_bool.compare_exchange_strong(expected, true))
							{
								self->SetException(e);
							}
						}
						else if (auto number = self->_counter; number == self->_tasks.size())
						{
							if (bool expected = false; self->_bool.compare_exchange_strong(expected, true))
							{
								self->SetResult(t);
							}
						}
					}, sToken);
				}
			}
		};

		return Task<>(Linq::ToVector(&tasks));
	}

	template<class... TTasks>
	static auto WhenAny(TTasks... tasks) requires
		(std::constructible_from<Task<>, TTasks> && ...)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");

		using BaseVariadic = GetVariadic_t<0, TTasks...>;
		if constexpr (CompatibleVariadic<BaseVariadic, TTasks...>)
		{
			std::array<BaseVariadic, sizeof...(TTasks)> tasksArray{ BaseVariadic(std::move(tasks))... };
			return WhenAny(tasksArray);
		}
		else
		{
			std::array<Task<>, sizeof...(TTasks)> tasksArray{ Task<>(std::move(tasks))... };
			return WhenAny(tasksArray);
		}
	}
};

template<>
void CORE_API Task<>::_Initialize();

template<>
void CORE_API Task<>::_Shutdown();

template<>
void CORE_API Task<>::_Run_thread(std::function<void()> body);

template<>
void CORE_API Task<>::_Delay_thread(std::chrono::milliseconds delay, std::function<void()> body);