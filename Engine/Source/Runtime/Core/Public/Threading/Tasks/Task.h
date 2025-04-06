// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "VoidableVector.h"
#include "TimeSpan.h"
#include "Threading/Tasks/promise_type.h"
#include "Threading/Tasks/Awaiter.h"
#include "Threading/ThreadPool.h"
#include "Threading/CancellationToken.h"
#include "Linq/Linq.ToVector.h"
#include <memory>

namespace Ayla
{
	template<class T = void>
	class [[nodiscard]] Task
	{
		template<class U>
		friend class Task;

	public:
		using promise_type = ::Ayla::promise_type<T, Task<T>>;
		using Awaiter_t = ::Ayla::Awaiter<T>;
		using ValueType = T;

	private:
		static bool bConfigureDefault;
		std::shared_ptr<AwaiterBase> Awaiter;

	public:
		Task() = default;
		Task(const Task&) = default;

		template<class U>
		explicit Task(std::shared_ptr<U> InAwaiter) requires
			std::constructible_from<Task, std::shared_ptr<U>, int>
			: Task(InAwaiter, 0)
		{
		}

		Task(std::shared_ptr<Awaiter_t> InAwaiter, int)
			: Awaiter(std::move(InAwaiter))
		{
		}

		explicit Task(std::shared_ptr<AwaiterBase> InAwaiter, short)
			: Awaiter(InAwaiter)
		{
		}

		explicit Task(const Task<>& InTask) requires (!std::same_as<T, void>)
			: Task(InTask.GetAwaiter())
		{
		}

		Task(Task&& InTask)
			: Awaiter(std::move(InTask.Awaiter))
		{
		}

		inline bool IsValid() const noexcept
		{
			return (bool)Awaiter;
		}

		inline std::shared_ptr<AwaiterBase> GetAwaiter() const requires std::is_void_v<T>
		{
			return Awaiter;
		}

		inline std::shared_ptr<Awaiter_t> GetAwaiter() const requires (!std::is_void_v<T>)
		{
			return std::static_pointer_cast<Awaiter_t>(Awaiter);
		}

		inline TaskStatus GetStatus() const noexcept
		{
			return Awaiter->GetStatus();
		}

		inline std::exception_ptr GetException() const noexcept
		{
			return Awaiter->GetException();
		}

		inline void RethrowException() const
		{
			std::rethrow_exception(Awaiter->GetException());
		}

		inline void AddStopCallback(CancellationToken sToken, std::function<void()> CallbackBody)
		{
			Awaiter->AddStopCallback(std::move(sToken), std::move(CallbackBody));
		}

		template<class TBody>
		auto ContinueWith(TBody&& ContinuationBody, CancellationToken sToken = {}) const -> Task<std::invoke_result_t<TBody, Task>>
		{
			using U = std::invoke_result_t<TBody, Task>;
			std::shared_ptr uAwaiter = std::make_shared<::Ayla::Awaiter<U>>(sToken);
			Awaiter->ContinueWith([ContinuationBody = std::forward<TBody>(ContinuationBody), uAwaiter](std::shared_ptr<AwaiterBase> result) mutable
				{
					try
					{
						if constexpr (std::same_as<U, void>)
						{
							ContinuationBody(Task(result));
							uAwaiter->SetResult();
						}
						else
						{
							auto r = ContinuationBody(Task(result));
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

		Task<bool> SuppressCancellationThrow() requires std::same_as<T, void>;
		Task<std::optional<T>> SuppressCancellationThrow() requires (!std::same_as<T, void>);

		inline void Wait() const noexcept
		{
			Awaiter->Wait();
		}

		inline Task<T> WaitAsync(CancellationToken cancellationToken) const noexcept
		{
			return ContinueWith([](Task<T> task)
				{
					return task.GetResult();
				}, cancellationToken);
		}

		inline bool WaitFor(const TimeSpan& Timeout) const noexcept
		{
			return Awaiter->WaitFor(Timeout);
		}

		inline T GetResult() const
		{
			if constexpr (std::same_as<T, void>)
			{
				Awaiter.get()->Wait();
				if (Awaiter->GetException())
				{
					std::rethrow_exception(Awaiter->GetException());
				}
			}
			else
			{
				return static_cast<Awaiter_t*>(Awaiter.get())->GetResult();
			}
		}

		inline bool IsCompleted() const noexcept
		{
			return Awaiter && Awaiter->IsCompleted();
		}

		inline bool IsCompletedSuccessfully() const noexcept
		{
			return Awaiter && Awaiter->GetStatus() == TaskStatus::RanToCompletion;
		}

		inline bool IsCanceled() const noexcept
		{
			return Awaiter && Awaiter->GetStatus() == TaskStatus::Canceled;
		}

		inline bool IsFaulted() const noexcept
		{
			return Awaiter && Awaiter->GetStatus() == TaskStatus::Faulted;
		}

		Task& operator =(const Task&) = default;
		Task& operator =(Task&&) = default;

		template<class U>
		explicit operator Task<U>() const requires
			std::same_as<T, void>
		{
			return Task<U>(Awaiter);
		}

		template<class U>
		operator Task<U>() const requires
			std::same_as<U, void> &&
			(!std::same_as<T, void>)
		{
			return Task<U>(Awaiter);
		}

		auto operator <=>(const Task&) const = default;
		bool operator ==(const Task&) const = default;

	public:
		template<class TBody>
		static auto Run(TBody&& Body, CancellationToken sToken = {}) -> Task<std::invoke_result_t<TBody>>
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");

			using U = std::invoke_result_t<TBody>;
			std::shared_ptr uAwaiter = std::make_shared<::Ayla::Awaiter<U>>(sToken);

			ThreadPool::QueueUserWorkItem([uAwaiter, Body = std::forward<TBody>(Body)]() mutable
				{
					try
					{
						if constexpr (std::same_as<U, void>)
						{
							Body();
							uAwaiter->SetResult();
						}
						else
						{
							U result = Body();
							uAwaiter->SetResult(std::move(result));
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

			std::shared_ptr uAwaiter = std::make_shared<::Ayla::Awaiter<void>>();
			ThreadPool::QueueUserWorkItem([uAwaiter]
			{
				uAwaiter->SetResult();
			});

			return Task<>(std::move(uAwaiter));
		}

		static Task<> Delay(std::chrono::milliseconds InDelay, CancellationToken sToken = {})
		{
			static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");

			std::shared_ptr uAwaiter = std::make_shared<::Ayla::Awaiter<void>>(sToken);
			ThreadPool::QueueDelayedUserWorkItem(InDelay, [uAwaiter]() mutable
			{
				uAwaiter->SetResult();
			});

			return Task<>(std::move(uAwaiter));
		}

		static Task<> CompletedTask()
		{
			static_assert(std::same_as<T, void>, "Use Task<>::CompletedTask instead.");

			static thread_local std::shared_ptr sAwaiter = []
			{
				auto ptr = std::make_shared<::Ayla::Awaiter<void>>();
				ptr->SetResult();
				return ptr;
			}();

			return Task<>(sAwaiter);
		}

		template<class U> requires (!std::same_as<U, void>)
			static Task<U> FromResult(U InValue)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::FromResult<U> instead.");

			auto Ptr = std::make_shared<::Ayla::Awaiter<U>>();
			Ptr->SetResult(std::move(InValue));
			return Task<U>(Ptr);
		}

		static void ConfigureDefault(bool bContinueOnCapturedContext)
		{
			static_assert(std::same_as<T, void>, "Use Task<>::ConfigureDefault instead.");
			bConfigureDefault = bContinueOnCapturedContext;
		}

	public:
		template<std::ranges::input_range R> requires std::convertible_to<std::ranges::range_value_t<R>, Task<>>
		static auto WhenAll(const R& InTasks)
		{
			static_assert(std::is_void_v<T>, "Use Task<>::WhenAll instead.");
			using RValueType = typename std::ranges::range_value_t<R>::ValueType;

			if (std::ranges::empty(InTasks))
			{
				if constexpr (std::is_void_v<RValueType>)
				{
					return Task<>::CompletedTask();
				}
				else
				{
					return Task<>::FromResult<std::vector<RValueType>>({});
				}
			}

			class WhenAllAwaiter : public ::Ayla::Awaiter<VoidableVector<RValueType>>
			{
				std::vector<Task<RValueType>> Tasks;

				std::atomic<size_t> Counter;
				std::atomic<bool> bComplete;
				VoidableVector<RValueType> Values;

			public:
				WhenAllAwaiter(std::vector<Task<RValueType>> Tasks)
					: ::Ayla::Awaiter<VoidableVector<RValueType>>()
					, Tasks(std::move(Tasks))
				{
					Values.resize(this->Tasks.size());
				}

				~WhenAllAwaiter() noexcept
				{
				}

				void Start(std::shared_ptr<WhenAllAwaiter> Self)
				{
					size_t Index = 0;
					for (auto& Task : Tasks)
					{
						std::ignore = Task.ContinueWith([Self, MyIndex = Index++](::Ayla::Task<RValueType> p)
						{
							if (p.IsCanceled())
							{
								if (bool bExpected = false; Self->bComplete.compare_exchange_strong(bExpected, true))
								{
									Self->Cancel();
								}
							}
							else if (auto E = p.GetException())
							{
								if (bool bExpected = false; Self->bComplete.compare_exchange_strong(bExpected, true))
								{
									Self->SetException(E);
								}
							}
							else
							{
								if constexpr (std::is_void_v<RValueType> == false)
								{
									Self->Values[MyIndex] = p.GetResult();
								}

								if (++Self->Counter == Self->Values.size())
								{
									if constexpr (std::is_void_v<RValueType>)
									{
										Self->SetResult();
									}
									else
									{
										Self->SetResult((std::vector<RValueType>&&)std::move(Self->Values));
									}
								}
							}
						});
					}
				}
			};

			auto Ptr = std::make_shared<WhenAllAwaiter>(InTasks | Linq::ToVector());
			Ptr->Start(Ptr);
			if constexpr (std::is_void_v<RValueType>)
			{
				return Task<>(std::move(Ptr));
			}
			else
			{
				return Task<VoidableVector<RValueType>>(std::move(Ptr)).ContinueWith([](Task<VoidableVector<RValueType>> p)
				{
					if constexpr (std::is_void_v<RValueType>)
					{
						// Task<>
						return;
					}
					else
					{
						return (std::vector<RValueType>)p.GetResult();
					}
				});
			}
		}

		template<std::convertible_to<Task<>> Task1, std::convertible_to<Task<>>... Tasks>
		static auto WhenAll(const Task1& InTask1, const Tasks&... InTasks)
		{
			static_assert(std::is_void_v<T>, "Use Task<>::WhenAll instead.");

			using ValueType1 = typename Task1::ValueType;
			constexpr bool bIsSameValues = (true && ... && std::same_as<ValueType1, typename Tasks::ValueType>);

			if constexpr (bIsSameValues && std::is_void_v<ValueType1> == false)
			{
				return WhenAll(std::array<Task<ValueType1>, sizeof...(Tasks) + 1>{ InTask1, InTasks... });
			}
			else
			{
				return WhenAll(std::array<Task<>, sizeof...(Tasks) + 1>{ InTask1, InTasks... });
			}
		}

		template<std::ranges::input_range R> requires std::convertible_to<std::ranges::range_value_t<R>, Task<>>
		static auto WhenAny(const R& InTasks)
		{
			static_assert(std::is_void_v<T>, "Use Task<>::WhenAll instead.");
			using ValueType = typename std::ranges::range_value_t<R>::ValueType;

			if (std::ranges::empty(InTasks))
			{
				throw InvalidOperationException(TEXT("Tasks of WhenAny is empty."));
			}

			class WhenAnyAwaiter : public ::Ayla::Awaiter<Task<ValueType>>
			{
				std::vector<Task<>> Tasks;
				std::atomic<bool> bComplete;

			public:
				WhenAnyAwaiter(std::vector<Task<ValueType>> Tasks)
					: ::Ayla::Awaiter<Task<ValueType>>()
					, Tasks(std::move(Tasks))
				{
				}

				void Start(std::shared_ptr<WhenAnyAwaiter> Self)
				{
					for (auto& Task : Tasks)
					{
						Task.ContinueWith([Self](::Ayla::Task<ValueType> p)
						{
							if (p.IsCanceled())
							{
								if (bool bExpected = false; Self->bComplete.compare_exchange_strong(bExpected, true))
								{
									Self->Cancel();
								}
							}
							else if (auto E = p.GetException())
							{
								if (bool bExpected = false; Self->bComplete.compare_exchange_strong(bExpected, true))
								{
									Self->SetException(E);
								}
							}
							else if (p.IsCompletedSuccessfully())
							{
								if (bool bExpected = false; Self->bComplete.compare_exchange_strong(bExpected, true))
								{
									Self->SetResult(p);
								}
							}
						});
					}
				}
			};

			auto Ptr = std::make_shared<WhenAnyAwaiter>(InTasks | Linq::ToVector());
			Ptr->Start(Ptr);
			return Task<Task<ValueType>>(std::move(Ptr));
		}

		template<std::convertible_to<Task<>> Task1, std::convertible_to<Task<>>... Tasks>
		static auto WhenAny(const Task1& InTask1, const Tasks&... InTasks)
		{
			static_assert(std::is_void_v<T>, "Use Task<>::WhenAny instead.");

			using ValueType1 = typename Task1::ValueType;
			constexpr bool bIsSameValues = (true && ... && std::same_as<ValueType1, typename Tasks::ValueType>);

			if constexpr (bIsSameValues)
			{
				return WhenAny(std::array<ValueType1, sizeof...(Tasks) + 1>{ InTask1, InTasks... });
			}
			else
			{
				return WhenAny(std::array<Task<>, sizeof...(Tasks) + 1>{ InTask1, InTasks... });
			}
		}
	};

	template<class T>
	Task<bool> Task<T>::SuppressCancellationThrow() requires std::same_as<T, void>
	{
		return ContinueWith([](Task<void> task)
		{
			if (task.GetStatus() == TaskStatus::Canceled)
			{
				return false;
			}
			else
			{
				return true;
			}
		});
	}

	template<class T>
	Task<std::optional<T>> Task<T>::SuppressCancellationThrow() requires (!std::same_as<T, void>)
	{
		return ContinueWith([](Task<T> task) -> std::optional<T>
		{
			if (task.GetStatus() == TaskStatus::Canceled)
			{
				return std::nullopt;
			}
			else
			{
				return task.GetResult();
			}
		});
	}
}