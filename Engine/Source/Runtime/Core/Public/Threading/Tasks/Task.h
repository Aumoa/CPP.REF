// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Awaiter.h"
#include "promise_type.h"
#include "Generic/IList.h"
#include "Concepts/CompatibleVariadic.h"
#include "Concepts/GetVariadic.h"
#include "Exceptions/InvalidCastException.h"
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

	template<class T>
	explicit Task(std::shared_ptr<T> awaiter) requires
		std::constructible_from<Task, std::shared_ptr<T>, int>
		: Task(awaiter, 0)
	{
	}

	Task(std::shared_ptr<Awaiter> awaiter, int)
		: _awaiter(std::move(awaiter))
	{
	}

	explicit Task(std::shared_ptr<AwaiterBase> awaiter, short)
		: _awaiter(awaiter)
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

	inline auto GetAwaiter() const noexcept(!std::same_as<T, void>)
	{
		if constexpr (std::same_as<T, void>)
		{
			return _awaiter;
		}
		else
		{
			return std::static_pointer_cast<Awaiter>(_awaiter);
		}
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
	auto ContinueWith(TBody&& body, std::stop_token sToken = {}) const -> Task<std::invoke_result_t<TBody, Task>>
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
			catch (...)
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

	inline bool WaitFor(const TimeSpan& timeout) const noexcept
	{
		return _awaiter->WaitFor(timeout);
	}

	inline T GetResult() const
	{
		return static_cast<Awaiter*>(_awaiter.get())->GetResult();
	}

	inline bool IsCompleted() const noexcept
	{
		return static_cast<Awaiter*>(_awaiter.get())->IsCompleted();
	}

	inline bool IsCompletedSuccessfully() const noexcept
	{
		return static_cast<Awaiter*>(_awaiter.get())->IsCompletedSuccessfully();
	}

	inline bool IsCanceled() const noexcept
	{
		return static_cast<Awaiter*>(_awaiter.get())->IsCanceled();
	}

	inline bool IsFaulted() const noexcept
	{
		return static_cast<Awaiter*>(_awaiter.get())->IsFaulted();
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
	static void Initialize();
	static void Shutdown();

	static void Run_thread(std::function<void()> body);
	static void Delay_thread(std::chrono::milliseconds delay, std::function<void()> body);

public:
	template<class TBody>
	static auto Run(TBody&& body, std::stop_token sToken = {}) -> Task<std::invoke_result_t<TBody>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");

		using U = std::invoke_result_t<TBody>;
		std::shared_ptr uAwaiter = std::make_shared<::Awaiter<U>>(sToken);

		Run_thread([uAwaiter, body = std::forward<TBody>(body)]() mutable
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
		Run_thread([uAwaiter]
		{
			uAwaiter->SetResult();
		});
		
		return Task<>(std::move(uAwaiter));
	}

	static Task<> Delay(std::chrono::milliseconds delay, std::stop_token sToken = {})
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");

		std::shared_ptr uAwaiter = std::make_shared<::Awaiter<void>>(sToken);
		Delay_thread(delay, [uAwaiter]() mutable
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
	static Task<U> FromResult(U value)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::FromResult<U> instead.");
		 
		auto ptr = std::make_shared<::Awaiter<U>>();
		ptr->SetResult(std::move(value));
		return Task<U>(ptr);
	}

	static void CancelAfter(std::stop_source ss, std::chrono::milliseconds delay)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::CancelAfter instead.");

		Delay(delay, ss.get_token()).ContinueWith([ss = std::move(ss)](auto) mutable
		{
			ss.request_stop();
		});
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
			WhenAllAwaiter(std::stop_token sToken)
				: ::Awaiter<void>(sToken)
			{
			}

			void Start(std::shared_ptr<WhenAllAwaiter> self, std::vector<Task<>> tasks)
			{
				check(tasks.empty() == false);
				_tasks = std::move(tasks);

				if (_tasks.empty())
				{
					this->SetResult();
					return;
				}

				for (auto& task : _tasks)
				{
					task.ContinueWith([self](Task<> t)
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
						else if (size_t number = ++self->_counter; number == self->_tasks.size())
						{
							self->SetResult();
						}
					});
				}
			}
		};

		auto ptr = std::make_shared<WhenAllAwaiter>(sToken);
		ptr->Start(ptr, Linq::ToVector(&tasks));
		return Task<>(std::move(ptr));
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
			WhenAllAwaiter(std::stop_token sToken)
				: ::Awaiter<void>(sToken)
			{
			}

			void Start(std::shared_ptr<WhenAllAwaiter> self, std::vector<Task<>> tasks)
			{
				check(tasks.empty() == false);
				_tasks = std::move(tasks);

				if (_tasks.empty())
				{
					this->SetResult();
					return;
				}

				_values = std::make_unique<char[]>(sizeof(ValueType) * _tasks.size());
				ValueType* values = reinterpret_cast<ValueType*>(_values.get());

				for (size_t i = 0; i < _tasks.size(); ++i)
				{
					_tasks[i].ContinueWith([self, values, i](Task<> t)
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
						else if (size_t number = ++self->_counter; number == self->_tasks.size())
						{
							new(values + i) ValueType(t.GetResult());
							if (number == self->_tasks.size())
							{
								self->SetResult(std::vector(values, values + self->_tasks.size()));
							}
						}
					});
				}
			}
		};

		auto ptr = std::make_shared<WhenAllAwaiter>(sToken);
		ptr->Start(ptr, Linq::ToVector(&tasks));
		return Task<>(std::move(ptr));
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
	static Task<EnumerableItem_t<TTasks>> WhenAny(TTasks tasks, std::stop_token sToken = {}) requires
		IEnumerable<TTasks, EnumerableItem_t<TTasks>>&&
		std::constructible_from<Task<>, EnumerableItem_t<TTasks>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAny instead.");

		using TTask = EnumerableItem_t<TTasks>;
		class WhenAnyAwaiter : public ::Awaiter<TTask>
		{
			std::vector<Task<>> _tasks;
			std::atomic<bool> _bool;

		public:
			WhenAnyAwaiter(std::stop_token sToken)
				: ::Awaiter<TTask>(sToken)
			{
			}

			void Start(std::shared_ptr<WhenAnyAwaiter> self, std::vector<Task<>> tasks)
			{
				_tasks = std::move(tasks);

				if (_tasks.empty())
				{
					this->SetResult();
					return;
				}

				for (auto& task : _tasks)
				{
					task.ContinueWith([self](Task<> t)
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
						else if (t.IsCompletedSuccessfully())
						{
							if (bool expected = false; self->_bool.compare_exchange_strong(expected, true))
							{
								self->SetResult(t);
							}
						}
					});
				}
			}
		};

		auto ptr = std::make_shared<WhenAnyAwaiter>(sToken);
		ptr->Start(ptr, Linq::ToVector(&tasks));
		return Task<TTask>(std::move(ptr));
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
void CORE_API Task<>::Initialize();

template<>
void CORE_API Task<>::Shutdown();

template<>
void CORE_API Task<>::Run_thread(std::function<void()> body);

template<>
void CORE_API Task<>::Delay_thread(std::chrono::milliseconds delay, std::function<void()> body);