// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IAwaitable.h"
#include "VoidableOptional.h"
#include <coroutine>

namespace Threading::Tasks
{
	template<class T>
	class AwaiterBase : public IAwaitable<T>
	{
		std::mutex Mutex;
		std::condition_variable Cvar;
		VoidableOptional<T> Value;
		std::vector<std::function<void(T)>> ThenProc;
		std::future<void> Runner;

	public:
		using ReturnType = T;

	public:
		AwaiterBase()
		{
		}

		virtual bool IsReady() const override
		{
			return Value.has_value();
		}

		virtual void Wait() override
		{
			if (!Value.has_value())
			{
				std::unique_lock Mutex_lock(Mutex);
				Cvar.wait(Mutex_lock);
			}
		}

		virtual T GetValue() override
		{
			Wait();
			return Value.value();
		}

		virtual void Then(std::function<void(T)> Proc) override
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

		virtual void SetRunner(std::future<void> Runner) override
		{
			this->Runner = std::move(Runner);
		}

		virtual std::future<void> GetRunner() override
		{
			return std::move(this->Runner);
		}

		template<class U>
		void SetValue(U&& Value) requires (!std::same_as<T, void>)
		{
			std::unique_lock Mutex_lock(Mutex);
			this->Value.emplace(std::forward<U>(Value));
			Cvar.notify_all();

			std::vector<std::function<void(T)>> Procedures;
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

			std::vector<std::function<void(T)>> Procedures;
			std::swap(Procedures, ThenProc);
			Mutex_lock.unlock();

			for (auto& Proc : Procedures)
			{
				Proc();
			}
		}

	public:
		virtual bool await_ready() override
		{
			return IsReady();
		}

		virtual void await_suspend(std::coroutine_handle<> Coroutine) noexcept override
		{
			Then([Coroutine = std::move(Coroutine)](T)
			{
				Coroutine.resume();
			});
		}

		virtual T await_resume() noexcept override
		{
			return GetValue();
		}
	};
}