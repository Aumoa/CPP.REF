// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoidableOptional.h"
#include "VoidableFunction.h"
#include <coroutine>

namespace Threading::Tasks
{
	template<class T>
	class Awaiter
	{
		std::mutex Mutex;
		std::condition_variable Cvar;
		VoidableOptional<T> Value;
		std::vector<VoidableFunction<T>> ThenProc;

	public:
		using ReturnType = T;

	public:
		Awaiter()
		{
		}

		bool IsReady() const
		{
			return Value.has_value();
		}

		void Wait()
		{
			if (!Value.has_value())
			{
				std::unique_lock Mutex_lock(Mutex);
				Cvar.wait(Mutex_lock);
			}
		}

		T GetValue()
		{
			Wait();
			return Value.value();
		}

		void Then(VoidableFunction<T> Proc)
		{
			std::unique_lock Mutex_lock(Mutex);
			if (Value.has_value())
			{
				Mutex_lock.unlock();
				if constexpr (std::same_as<T, void>)
				{
					Proc();
				}
				else
				{
					Proc(*Value);
				}
			}
			else
			{
				ThenProc.emplace_back(std::move(Proc));
			}
		}

		template<class U>
		void SetValue(U&& Value) requires (!std::same_as<T, void>)
		{
			std::unique_lock Mutex_lock(Mutex);
			this->Value.emplace(std::forward<U>(Value));
			Cvar.notify_all();

			std::vector<VoidableFunction<T>> Procedures;
			std::swap(Procedures, ThenProc);
			Mutex_lock.unlock();

			for (auto& Proc : Procedures)
			{
				Proc(*this->Value);
			}
		}

		void SetValue() requires (std::same_as<T, void>)
		{
			std::unique_lock Mutex_lock(Mutex);
			this->Value.emplace();
			Cvar.notify_all();

			std::vector<VoidableFunction<T>> Procedures;
			std::swap(Procedures, ThenProc);
			Mutex_lock.unlock();

			for (auto& Proc : Procedures)
			{
				Proc();
			}
		}

	public:
		bool await_ready()
		{
			return IsReady();
		}

		template<class TCoroutineHandle>
		void await_suspend(TCoroutineHandle Coroutine) noexcept
		{
			if constexpr (std::same_as<T, void>)
			{
				Then([Coroutine = std::move(Coroutine)]()
				{
					Coroutine.resume();
				});
			}
			else
			{
				Then([Coroutine = std::move(Coroutine)](T)
				{
					Coroutine.resume();
				});
			}
		}

		T await_resume() noexcept
		{
			return GetValue();
		}
	};
}