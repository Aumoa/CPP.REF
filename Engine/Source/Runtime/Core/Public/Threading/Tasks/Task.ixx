// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Threading:Task;

export import :promise_type;
export import :Awaiter;
export import :ThreadPool;
export import :SynchronizationContext;

export template<class T = void>
class [[nodiscard]] Task
{
	template<class U>
	friend class Task;
	friend class Core;

public:
	using promise_type = ::promise_type<T, Task<T>>;
	using Awaiter_t = ::Awaiter<T>;
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

	inline auto GetAwaiter() const noexcept(!std::same_as<T, void>)
	{
		if constexpr (std::same_as<T, void>)
		{
			return Awaiter;
		}
		else
		{
			return std::static_pointer_cast<Awaiter_t>(Awaiter);
		}
	}

	inline ETaskStatus GetStatus() const noexcept
	{
		return Awaiter->GetStatus();
	}

	inline std::exception_ptr GetException() const noexcept
	{
		return Awaiter->GetException();
	}

	inline void AddStopCallback(std::stop_token sToken, std::function<void()> CallbackBody)
	{
		Awaiter->AddStopCallback(std::move(sToken), std::move(CallbackBody));
	}
	
	template<class TBody>
	auto ContinueWith(TBody&& ContinuationBody, std::stop_token sToken = {}) const -> Task<std::invoke_result_t<TBody, Task>>
	{
		using U = std::invoke_result_t<TBody, Task>;
		std::shared_ptr uAwaiter = std::make_shared<::Awaiter<U>>(sToken);
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

	auto ConfigureAwait(bool bContinueOnCapturedContext = bConfigureDefault)
	{
		if (bContinueOnCapturedContext == false)
		{
			return *this;
		}

		auto Current = SynchronizationContext::GetCurrent();
		if (Current == nullptr)
		{
			return *this;
		}

		std::shared_ptr uAwaiter = std::make_shared<::Awaiter<T>>();
		ContinueWith([Current, uAwaiter](auto Previous)
		{
			Current->Enqueue([uAwaiter, Previous]()
			{
				if constexpr (std::same_as<T, void>)
				{
					uAwaiter->SetResult();
				}
				else
				{
					uAwaiter->SetResult(Previous.GetResult());
				}
			});
		});

		return Task(std::move(uAwaiter));
	}

	inline void Wait() const noexcept
	{
		Awaiter->Wait();
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
		return Awaiter->IsCompleted();
	}

	inline bool IsCompletedSuccessfully() const noexcept
	{
		return Awaiter->GetStatus() == ETaskStatus::RanToCompletion;
	}

	inline bool IsCanceled() const noexcept
	{
		return Awaiter->GetStatus() == ETaskStatus::Canceled;
	}

	inline bool IsFaulted() const noexcept
	{
		return Awaiter->GetStatus() == ETaskStatus::Faulted;
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
	static auto Run(TBody&& Body, std::stop_token sToken = {}) -> Task<std::invoke_result_t<TBody>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");

		using U = std::invoke_result_t<TBody>;
		std::shared_ptr uAwaiter = std::make_shared<::Awaiter<U>>(sToken);

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
		ThreadPool::QueueUserWorkItem([uAwaiter]
		{
			uAwaiter->SetResult();
		});
		
		return Task<>(std::move(uAwaiter));
	}

	static Task<> Delay(std::chrono::milliseconds InDelay, std::stop_token sToken = {})
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Delay instead.");

		std::shared_ptr uAwaiter = std::make_shared<::Awaiter<void>>(sToken);
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
			auto ptr = std::make_shared<::Awaiter<void>>();
			ptr->SetResult();
			return ptr;
		}();

		return Task<>(sAwaiter);
	}

	template<class U> requires (!std::same_as<U, void>)
	static Task<U> FromResult(U InValue)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::FromResult<U> instead.");
		 
		auto Ptr = std::make_shared<::Awaiter<U>>();
		Ptr->SetResult(std::move(InValue));
		return Task<U>(Ptr);
	}

	static void CancelAfter(std::stop_source StopSource, std::chrono::milliseconds InDelay)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::CancelAfter instead.");

		Delay(InDelay, StopSource.get_token()).ContinueWith([StopSource = std::move(StopSource)](auto) mutable
		{
			StopSource.request_stop();
		});
	}

	static void ConfigureDefault(bool bContinueOnCapturedContext)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::ConfigureDefault instead.");
		bConfigureDefault = bContinueOnCapturedContext;
	}

public:
	template<IList<Task<>> TTasks>
	static Task<> WhenAll(TTasks InTasks, std::stop_token sToken = {}) requires
		std::constructible_from<Task<>, EnumerableItem_t<TTasks>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");

		class WhenAllAwaiter : public ::Awaiter<void>
		{
			std::vector<Task<>> Tasks;
			std::atomic<size_t> Counter;
			std::atomic<bool> bComplete;

		public:
			WhenAllAwaiter(std::stop_token sToken)
				: ::Awaiter<void>(sToken)
			{
			}

			void Start(std::shared_ptr<WhenAllAwaiter> Self, std::vector<Task<>> InTasks)
			{
				check(InTasks.empty() == false);
				Tasks = std::move(InTasks);

				if (Tasks.empty())
				{
					this->SetResult();
					return;
				}

				for (auto& Task : Tasks)
				{
					std::ignore = Task.ContinueWith([Self](::Task<> t)
					{
						if (t.IsCanceled())
						{
							if (bool expected = false; Self->bComplete.compare_exchange_strong(expected, true))
							{
								Self->Cancel();
							}
						}
						else if (auto e = t.GetException())
						{
							if (bool expected = false; Self->bComplete.compare_exchange_strong(expected, true))
							{
								Self->SetException(e);
							}
						}
						else if (size_t number = ++Self->Counter; number == Self->Tasks.size())
						{
							Self->SetResult();
						}
					});
				}
			}
		};

		if (InTasks.size() == 0)
		{
			return Task<>::CompletedTask();
		}

		auto Ptr = std::make_shared<WhenAllAwaiter>(sToken);
		Ptr->Start(Ptr, Linq::ToVector(&InTasks));
		return Task<>(std::move(Ptr));
	}
	
	template<class TTasks>
	static auto WhenAll(TTasks InTasks, std::stop_token sToken = {})
		-> Task<std::vector<typename EnumerableItem_t<TTasks>::ValueType>> requires
		IEnumerable<TTasks, EnumerableItem_t<TTasks>> &&
		std::constructible_from<Task<>, EnumerableItem_t<TTasks>> &&
		(!std::same_as<typename EnumerableItem_t<TTasks>::ValueType, void>)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");
		using ValueType = typename EnumerableItem_t<ValueType>::ValueType;

		class WhenAllAwaiter : public ::Awaiter<std::vector<ValueType>>
		{
			std::vector<Task<>> Tasks;
			std::atomic<size_t> Counter;
			std::atomic<bool> bComplete;
			std::unique_ptr<char[]> Values;

		public:
			WhenAllAwaiter(std::stop_token sToken)
				: ::Awaiter<void>(sToken)
			{
			}

			void Start(std::shared_ptr<WhenAllAwaiter> self, std::vector<Task<>> tasks)
			{
				check(tasks.empty() == false);
				Tasks = std::move(tasks);

				if (Tasks.empty())
				{
					this->SetResult();
					return;
				}

				Values = std::make_unique<char[]>(sizeof(ValueType) * Tasks.size());
				ValueType* values = reinterpret_cast<ValueType*>(Values.get());

				for (size_t i = 0; i < Tasks.size(); ++i)
				{
					Tasks[i].ContinueWith([self, values, i](Task<> t)
					{
						if (t.IsCanceled())
						{
							if (bool expected = false; self->bComplete.compare_exchange_strong(expected, true))
							{
								self->Cancel();
							}
						}
						else if (auto e = t.GetException())
						{
							if (bool expected = false; self->bComplete.compare_exchange_strong(expected, true))
							{
								self->SetException(e);
							}
						}
						else if (size_t number = ++self->Counter; number == self->Tasks.size())
						{
							new(values + i) ValueType(t.GetResult());
							if (number == self->Tasks.size())
							{
								self->SetResult(std::vector(values, values + self->Tasks.size()));
							}
						}
					});
				}
			}
		};

		auto Ptr = std::make_shared<WhenAllAwaiter>(sToken);
		Ptr->Start(Ptr, Linq::ToVector(&InTasks));
		return Task<>(std::move(Ptr));
	}

	template<class... TTasks>
	static auto WhenAll(TTasks... InTasks) requires
		(std::constructible_from<Task<>, TTasks> && ...)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");

		using BaseVariadic = GetVariadic_t<0, TTasks...>;
		if constexpr (CompatibleVariadic<BaseVariadic, TTasks...>)
		{
			std::array<BaseVariadic, sizeof...(TTasks)> TasksArray{ BaseVariadic(std::move(InTasks))... };
			return WhenAll(TasksArray);
		}
		else
		{
			std::array<Task<>, sizeof...(TTasks)> TasksArray{ Task<>(std::move(InTasks))... };
			return WhenAll(TasksArray);
		}
	}

	template<class TTasks>
	static Task<EnumerableItem_t<TTasks>> WhenAny(TTasks InTasks, std::stop_token sToken = {}) requires
		IEnumerable<TTasks, EnumerableItem_t<TTasks>>&&
		std::constructible_from<Task<>, EnumerableItem_t<TTasks>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAny instead.");

		using TTask = EnumerableItem_t<TTasks>;
		class WhenAnyAwaiter : public ::Awaiter<TTask>
		{
			std::vector<Task<>> Tasks;
			std::atomic<bool> bComplete;

		public:
			WhenAnyAwaiter(std::stop_token sToken)
				: ::Awaiter<TTask>(sToken)
			{
			}

			void Start(std::shared_ptr<WhenAnyAwaiter> self, std::vector<Task<>> InTasks)
			{
				Tasks = std::move(InTasks);

				if (Tasks.empty())
				{
					this->SetResult();
					return;
				}

				for (auto& Task : Tasks)
				{
					Task.ContinueWith([self](::Task<> t)
					{
						if (t.IsCanceled())
						{
							if (bool expected = false; self->bComplete.compare_exchange_strong(expected, true))
							{
								self->Cancel();
							}
						}
						else if (auto e = t.GetException())
						{
							if (bool expected = false; self->bComplete.compare_exchange_strong(expected, true))
							{
								self->SetException(e);
							}
						}
						else if (t.IsCompletedSuccessfully())
						{
							if (bool expected = false; self->bComplete.compare_exchange_strong(expected, true))
							{
								self->SetResult(t);
							}
						}
					});
				}
			}
		};

		check(Tasks.size() > 0);
		auto Ptr = std::make_shared<WhenAnyAwaiter>(sToken);
		Ptr->Start(Ptr, Linq::ToVector(&InTasks));
		return Task<TTask>(std::move(Ptr));
	}

	template<class... TTasks>
	static auto WhenAny(TTasks... InTasks) requires
		(std::constructible_from<Task<>, TTasks> && ...)
	{
		static_assert(std::same_as<T, void>, "Use Task<>::WhenAll instead.");

		using BaseVariadic = GetVariadic_t<0, TTasks...>;
		if constexpr (CompatibleVariadic<BaseVariadic, TTasks...>)
		{
			std::array<BaseVariadic, sizeof...(TTasks)> TasksArray{ BaseVariadic(std::move(InTasks))... };
			return WhenAny(TasksArray);
		}
		else
		{
			std::array<Task<>, sizeof...(TTasks)> TasksArray{ Task<>(std::move(InTasks))... };
			return WhenAny(TasksArray);
		}
	}
};