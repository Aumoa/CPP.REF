// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VirtualDtor.h"
#include <future>
#include <mutex>
#include <functional>

template<class T>
class ContinuousTask
{
	template<class U>
	friend class ContinuousTask;

	template<class U>
	struct VoidableOptional : public std::optional<T>
	{
		bool HasValue() const
		{
			return std::optional<T>::has_value();
		}

		void SetValue(std::future<T>& Fut)
		{
			std::optional<T>::operator =(Fut.get());
		}

		T GetValue() const
		{
			return std::optional<T>::operator *();
		}
	};

	template<>
	struct VoidableOptional<void>
	{
		bool bHasValue = false;

		bool HasValue() const
		{
			return bHasValue;
		}

		void SetValue(std::future<T>& Fut)
		{
			bHasValue = true;
		}

		T GetValue() const
		{
			return;
		}
	};

private:
	std::future<T> Task;
	VoidableOptional<T> Result;

	std::shared_ptr<std::mutex> ThenMtx;
	std::shared_ptr<std::function<void(T)>> ExecuteThen;
	std::vector<std::unique_ptr<VirtualDtor>> TaskDisposer;

public:
	ContinuousTask(ContinuousTask&& Rhs) noexcept
		: Task(std::move(Rhs.Task))
		, Result(std::move(Rhs.Result))
		, ThenMtx(std::move(Rhs.ThenMtx))
		, ExecuteThen(std::move(Rhs.ExecuteThen))
		, TaskDisposer(std::move(Rhs.TaskDisposer))
	{
	}

private:
	ContinuousTask(std::future<T> Task) : Task(std::move(Task))
	{
		ThenMtx = std::make_shared<std::mutex>();
		ExecuteThen = std::make_shared<std::function<void(T)>>();
	}

	ContinuousTask(std::function<T()> Body)
	{
		ThenMtx = std::make_shared<std::mutex>();
		ExecuteThen = std::make_shared<std::function<void(T)>>();

		Task = std::async(std::launch::async, [Body, ExecuteThen = ExecuteThen, ThenMtx = ThenMtx]() mutable
		{
			if constexpr (std::is_same_v<T, void>)
			{
				Body();

				{
					std::unique_lock ThenMtx_lock(*ThenMtx);

					if (*ExecuteThen)
					{
						std::remove_reference_t<decltype(*ExecuteThen)> Swap;
						std::swap(*ExecuteThen, Swap);
						Swap();
					}

					ExecuteThen.reset();
				}
			}
			else
			{
				T ReturnValue = Body();

				{
					std::unique_lock ThenMtx_lock(*ThenMtx);

					if (*ExecuteThen)
					{
						std::remove_reference_t<decltype(*ExecuteThen)> Swap;
						std::swap(*ExecuteThen, Swap);
						Swap(ReturnValue);
					}

					ExecuteThen.reset();
				}

				return ReturnValue;
			}
		});
	}

public:
	ContinuousTask()
	{
	}

	~ContinuousTask()
	{
	}

	inline void Wait()
	{
		check(IsValid());
		if (!Result.HasValue())
		{
			Task.wait();
		}
	}

	inline bool IsValid() const
	{
		return Result.HasValue() || Task.valid();
	}

	inline T GetResult()
	{
		check(IsValid());
		if (!Result.HasValue())
		{
			Result.SetValue(Task);
		}
		return Result.GetValue();
	}

	inline operator bool() const
	{
		return IsValid();
	}

public:
	static ContinuousTask<T> Run(std::function<T()> Body)
	{
		return ContinuousTask(Body);
	}

	template<class U>
	ContinuousTask<U> Then(std::function<U(T)> Body)
	{
		using namespace std::chrono_literals;

		std::unique_lock ThenMtx_lock(*ThenMtx);

		if (Task.wait_for(0s) == std::future_status::ready)
		{
			if constexpr (std::same_as<T, void>)
			{
				return ContinuousTask<U>(std::bind(Body));
			}
			else
			{
				return ContinuousTask<U>(std::bind(Body, Task.get()));
			}
		}
		else
		{
			std::shared_ptr Promise = std::make_shared<std::promise<U>>();
			ContinuousTask<U> Continue = Promise->get_future();

			if constexpr (std::same_as<T, void>)
			{
				*ExecuteThen = [this, Body, Promise, ThenMtx = Continue.ThenMtx, ExecuteThen = Continue.ExecuteThen](T) mutable
				{
					if constexpr (std::is_same_v<U, void>)
					{
						Body();
						Promise->set_value();

						std::unique_lock ThenMtx_lock(*ThenMtx);

						if (*ExecuteThen)
						{
							std::remove_reference_t<decltype(*ExecuteThen)> Swap;
							std::swap(*ExecuteThen, Swap);
							Swap();
						}
					}
					else
					{
						U ReturnValue = Body();
						Promise->set_value(ReturnValue);

						std::unique_lock ThenMtx_lock(*ThenMtx);

						if (*ExecuteThen)
						{
							std::remove_reference_t<decltype(*ExecuteThen)> Swap;
							std::swap(*ExecuteThen, Swap);
							Swap(ReturnValue);
						}
					}
				};
			}
			else
			{
				*ExecuteThen = [this, Body, Promise, ThenMtx = Continue.ThenMtx, ExecuteThen = Continue.ExecuteThen](T Previous) mutable
				{
					if constexpr (std::is_same_v<U, void>)
					{
						Body(std::move(Previous));
						Promise->set_value();

						std::unique_lock ThenMtx_lock(*ThenMtx);

						if (*ExecuteThen)
						{
							std::remove_reference_t<decltype(*ExecuteThen)> Swap;
							std::swap(*ExecuteThen, Swap);
							Swap();
						}
					}
					else
					{
						U ReturnValue = Body(std::move(Previous));
						Promise->set_value(ReturnValue);

						std::unique_lock ThenMtx_lock(*ThenMtx);

						if (*ExecuteThen)
						{
							std::remove_reference_t<decltype(*ExecuteThen)> Swap;
							std::swap(*ExecuteThen, Swap);
							Swap(ReturnValue);
						}
					}
				};
			}

			struct Disposable : public VirtualDtor
			{
				std::future<T> Storage;
				Disposable(std::future<T> Task) : Storage(std::move(Task))
				{
				}
			};

			Continue.TaskDisposer = std::move(TaskDisposer);
			Continue.TaskDisposer.emplace_back(std::make_unique<Disposable>(std::move(Task)));
			return Continue;
		}
	}
};