// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoidableOptional.h"
#include "VoidableFunction.h"
#include <coroutine>

namespace Threading::Tasks
{
	enum class EStatus
	{
		Running,
		Completed,
		Canceled,
	};

	template<class T>
	class Awaiter
	{
		std::mutex Mutex;
		std::condition_variable Cvar;
		VoidableOptional<T> Value;
		bool bCancel = false;

		std::vector<VoidableFunction<T>> ThenProc;
		std::vector<std::function<void()>> ElseProc;
		std::exception_ptr CaughtException;

	public:
		using ReturnType = T;

	public:
		Awaiter()
		{
		}

		bool IsReady() const
		{
			return !bCancel && Value.has_value();
		}

		void Wait()
		{
			if (!bCancel && !Value.has_value())
			{
				std::unique_lock Mutex_lock(Mutex);
				Cvar.wait(Mutex_lock);
			}

			CheckAndRethrow();
		}

		T GetValue()
		{
			Wait();
			check(!bCancel);
			CheckAndRethrow();
			return Value.value();
		}

		EStatus GetStatus() const
		{
			if (bCancel)
			{
				return EStatus::Canceled;
			}
			else if (Value.has_value())
			{
				return EStatus::Completed;
			}
			else
			{
				return EStatus::Running;
			}
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

		void Else(std::function<void()> Proc)
		{
			std::unique_lock Mutex_lock(Mutex);
			if (bCancel)
			{
				Mutex_lock.unlock();
				Proc();
			}
			else
			{
				ElseProc.emplace_back(std::move(Proc));
			}
		}

		template<class... U>
		void SetValue(U&&... Value)
		{
			std::unique_lock Mutex_lock(Mutex);
			if (bCancel)
			{
				return;
			}

			this->Value.emplace(Value...);
			Cvar.notify_all();

			std::vector<VoidableFunction<T>> Procedures;
			std::swap(Procedures, ThenProc);
			Mutex_lock.unlock();

			for (auto& Proc : Procedures)
			{
				Proc(Value...);
			}
		}

		bool Cancel()
		{
			std::unique_lock Mutex_lock(Mutex);
			if (!bCancel)
			{
				if (Value.has_value())
				{
					return false;
				}

				bCancel = true;
				Cvar.notify_all();

				std::vector<std::function<void()>> Procedures;
				std::swap(Procedures, ElseProc);
				Mutex_lock.unlock();

				for (auto& Proc : Procedures)
				{
					Proc();
				}
			}

			return true;
		}

		void SetExceptionPtr(std::exception_ptr Ptr)
		{
			CaughtException = std::move(Ptr);
		}

	private:
		void CheckAndRethrow()
		{
			if (CaughtException)
			{
				std::rethrow_exception(CaughtException);
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
			auto Awaiter = Coroutine.promise().Awaiter;

			if constexpr (std::same_as<T, void>)
			{
				Then([Coroutine, Awaiter]()
				{
					if (Awaiter->GetStatus() != EStatus::Canceled)
					{
						Coroutine.resume();
					}
				});
			}
			else
			{
				Then([Coroutine, Awaiter](T)
				{
					if (Awaiter->GetStatus() != EStatus::Canceled)
					{
						Coroutine.resume();
					}
				});
			}

			Else([Coroutine, Awaiter]()
			{
				Awaiter->Cancel();
			});
		}

		T await_resume() noexcept
		{
			return GetValue();
		}
	};
}