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

	class IAwaitable
	{
	public:
		virtual ~IAwaitable() noexcept = default;

		virtual bool IsReady() const = 0;
		virtual void Wait() = 0;
		virtual EStatus GetStatus() const = 0;
		virtual void Then(VoidableFunction<void> Proc) = 0;
		virtual void Else(VoidableFunction<void> Proc) = 0;
		virtual bool Cancel() = 0;
		virtual void SetExceptionPtr(std::exception_ptr Ptr) = 0;

		bool await_ready()
		{
			return IsReady();
		}

		template<class TCoroutineHandle>
		void await_suspend(TCoroutineHandle Coroutine) noexcept
		{
			auto Awaiter = Coroutine.promise().Awaiter;

			Else([Awaiter]()
			{
				Awaiter->Cancel();
			});

			Then([Coroutine, Awaiter]()
			{
				if (Awaiter->GetStatus() != EStatus::Canceled)
				{
					Coroutine.resume();
				}
			});
		}

		void await_resume() noexcept
		{
		}
	};

	template<class T>
	class Awaiter : public IAwaitable
	{
		std::mutex Mutex;
		std::condition_variable Cvar;
		VoidableOptional<T> Value;
		bool bCancel = false;

		std::vector<VoidableFunction<T>> ThenProc;
		std::vector<VoidableFunction<void>> ElseProc;
		std::exception_ptr CaughtException;

	public:
		using ReturnType = T;

	public:
		Awaiter()
		{
		}

		virtual bool IsReady() const override
		{
			return !bCancel && Value.has_value();
		}

		virtual void Wait() override
		{
			if (!bCancel && !Value.has_value())
			{
				std::unique_lock Mutex_lock(Mutex);
				Cvar.wait(Mutex_lock);
			}

			CheckAndRethrow();
		}

		virtual EStatus GetStatus() const override
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

		void Else(VoidableFunction<void> Proc)
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

		virtual void Then(VoidableFunction<void> Proc) override
		{
			if constexpr (std::same_as<T, void>)
			{
				std::unique_lock Mutex_lock(Mutex);
				if (Value.has_value())
				{
					Mutex_lock.unlock();
					Proc();
				}
				else
				{
					ThenProc.emplace_back(std::move(Proc));
				}
			}
			else
			{
				Then([Proc](T) mutable
				{
					Proc();
				});
			}
		}

		template<class U = T> requires (!std::same_as<U, void>)
		void Then(VoidableFunction<T> Proc)
		{
			std::unique_lock Mutex_lock(Mutex);
			if (Value.has_value())
			{
				Mutex_lock.unlock();
				Proc(*Value);
			}
			else
			{
				ThenProc.emplace_back(std::move(Proc));
			}
		}

		virtual bool Cancel() override
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

		virtual void SetExceptionPtr(std::exception_ptr Ptr) override
		{
			CaughtException = std::move(Ptr);
		}

		T GetValue()
		{
			Wait();
			checkf(!bCancel, L"Task is already canceled.");
			CheckAndRethrow();
			return Value.value();
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

	private:
		void CheckAndRethrow()
		{
			if (CaughtException)
			{
				std::rethrow_exception(CaughtException);
			}
		}

	public:
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