// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Awaiter.h"
#include "CoreConcepts.h"
#include <coroutine>
#include <memory>
#include <chrono>
#include <array>

namespace libty::inline Core
{
	class ThreadGroup;
}

namespace libty::inline Core
{
	template<class T>
	class Task;

	namespace Impl
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
		using promise_type = libty::Impl::promise_type<T>;
		using TGenericVoid = void;

	protected:
		using MyAwaiter = Awaiter<T>;
		using SourceAwaiter = typename libty::Impl::SourceAwaiter<T>::type;
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
			return ((Awaiter<T>*)_awaiter.get())->GetResult();
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

		auto operator <=>(const Task&) const = default;
		bool operator ==(const Task&) const = default;

	private:
		void ThrowInvalid()
		{
			if (!IsValid())
			{
				throw InvalidOperationException("Task is null.");
			}
		}

	public:
		template<class TBody>
		auto ContinueWith(TBody&& body, std::source_location source = std::source_location::current()) -> Task<FunctionReturn_t<TBody, Task>>
		{
			using U = FunctionReturn_t<TBody, Task>;

			ThrowInvalid();
			std::shared_ptr uAwaiter = std::make_shared<Awaiter<U>>();

			_awaiter->Then([body = std::forward<TBody>(body), uAwaiter, source](Task<> result) mutable
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
			throw;
		}

		static void DelayImpl(std::chrono::milliseconds delay, std::function<void()> body)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");
			throw;
		}

	public:
		template<class TBody>
		static auto Run(TBody&& body, std::stop_token cancellationToken = {}, std::source_location source = std::source_location::current()) -> Task<FunctionReturn_t<TBody>>
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");
			throw;
		}

		static auto Yield(std::source_location source = std::source_location::current())
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Yield instead.");
			throw;
		}

		static Task<> Delay(std::chrono::milliseconds delay, std::stop_token cancellationToken = {}, std::source_location source = std::source_location::current())
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");
			throw;
		}

		static Task<> CompletedTask()
		{
			static_assert(std::same_as<T, void>, "Use Task<>::CompletedTask instead.");
			throw;
		}

		template<class U> requires (!std::same_as<U, void>)
		static Task<> FromResult(U value, std::source_location source = std::source_location::current())
		{
			static_assert(std::same_as<T, void>, "Use Task<>::FromResult<U> instead.");
			throw;
		}

	public:
		template<class TTasks>
		static Task<> WhenAll(TTasks tasks, std::stop_token cancellationToken = {}) requires
			IList<TTasks, EnumerableItem_t<TTasks>> &&
			std::constructible_from<Task<>, EnumerableItem_t<TTasks>>
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");
			class WhenAllAwaiter : public Awaiter<void>
			{
				size_t _desired;
				std::atomic<size_t> _counter;
				std::atomic<bool> _stored;

			public:
				WhenAllAwaiter(size_t desired)
					: _desired(desired)
				{
				}

				bool Join(Task<> result)
				{
					if (_stored.load())
					{
						return false;
					}

					if (result.GetStatus() == ETaskStatus::Faulted)
					{
						bool expected = false;
						if (_stored.compare_exchange_strong(expected, true))
						{
							Awaiter<void>::SetException(result.GetException());
						}
					}
					else
					{
						size_t counter = ++_counter;
						if (counter == _desired)
						{
							bool expected = false;
							if (_stored.compare_exchange_strong(expected, true))
							{
								Awaiter<void>::SetResult();
								return true;
							}
						}
					}

					return false;
				}

				void Cancel()
				{
					bool expected = false;
					if (_stored.compare_exchange_strong(expected, true))
					{
						Awaiter<void>::Cancel();
					}
				}
			};

			std::shared_ptr awaiter = std::make_shared<WhenAllAwaiter>(std::size(tasks));
			for (auto& task : tasks)
			{
				task.ContinueWith([awaiter, cancellationToken](Task<> result) mutable
				{
					bool completed = awaiter->Join(result);
					if (!completed && cancellationToken.stop_possible() && cancellationToken.stop_requested())
					{
						awaiter->Cancel();
					}
				});
			}

			return awaiter;
		}

		template<class... TTasks>
		static Task<> WhenAll(TTasks... tasks) requires
			(std::constructible_from<Task<>, TTasks> && ...)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");
			std::array<Task<>, sizeof...(TTasks)> tasksArray{ Task<>(std::move(tasks))... };
			return WhenAll(tasksArray);
		}

		template<class TTasks>
		static Task<Task<>> WhenAny(TTasks tasks) requires
			IEnumerable<TTasks, EnumerableItem_t<TTasks>>&&
			std::constructible_from<Task<>, EnumerableItem_t<TTasks>>
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAny instead.");
			class WhenAnyAwaiter : public Awaiter<Task<>>
			{
				std::atomic<bool> _alreadyBound;

			public:
				void Join(Task<> result)
				{
					bool expected = false;
					if (_alreadyBound.compare_exchange_strong(expected, true))
					{
						SetResult(std::move(result));
					}
				}
			};

			std::shared_ptr awaiter = std::make_shared<WhenAnyAwaiter>();
			for (auto& task : tasks)
			{
				task.ContinueWith([awaiter](Task<> result) mutable
				{
					awaiter->Join(std::move(result));
				});
			}

			return Task<Task<>>(awaiter);
		}

		template<class... TTasks>
		static Task<Task<>> WhenAny(TTasks... tasks) requires
			(std::constructible_from<Task<>, TTasks> && ...)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::WhenAny instead.");
			std::array<Task<>, sizeof...(TTasks)> tasksArray{ Task<>(std::move(tasks))... };
			return WhenAny(tasksArray);
		}
	};

	template<>
	void CORE_API Task<>::RunImpl(std::function<void()> body);

	template<>
	void CORE_API Task<>::DelayImpl(std::chrono::milliseconds delay, std::function<void()> body);

	template<>
	template<class TBody>
	inline auto Task<>::Run(TBody&& body, std::stop_token cancellationToken, std::source_location source) -> Task<FunctionReturn_t<TBody>>
	{
		using U = FunctionReturn_t<TBody>;
		std::shared_ptr awaiter = std::make_shared<Awaiter<U>>();
		awaiter->WaitingToRun();

		RunImpl([awaiter, body = std::forward<TBody>(body), cancellationToken, source]() mutable
		{
			if (cancellationToken.stop_possible() && cancellationToken.stop_requested())
			{
				awaiter->Cancel();
				return;
			}

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
	inline auto Task<>::Yield(std::source_location source)
	{
		return Run([] {}, {}, source);
	}

	template<>
	inline Task<> Task<>::Delay(std::chrono::milliseconds delay, std::stop_token cancellationToken, std::source_location source)
	{
		std::shared_ptr awaiter = std::make_shared<Awaiter<void>>();
		awaiter->WaitingToRun();

		DelayImpl(delay, [awaiter, cancellationToken, source]() mutable
		{
			if (cancellationToken.stop_possible() && cancellationToken.stop_requested())
			{
				awaiter->Cancel();
				return;
			}

			awaiter->Running();
			awaiter->SetResult((const std::source_location&)source);
		});

		return Task<>(std::move(awaiter));
	}

	template<>
	inline Task<> Task<>::CompletedTask()
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
	inline Task<> Task<>::FromResult(U value, std::source_location source)
	{
		auto ptr = std::make_shared<Awaiter<U>>();
		ptr->SetResult(std::move(value), source);
		return ptr;
	}
}