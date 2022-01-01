// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include <future>
#include <coroutine>
#include <exception>
#include "AwaitableTask.h"
#include "ContinuousTask.h"

template<class T = void>
class Task
{
	template<class U>
	friend class Task;
	template<class U, template<class> class TTask>
	friend struct AwaitableTask;

public:
	using promise_type = AwaitableTask<T, Task>;
	using MyCoroutine = std::coroutine_handle<promise_type>;
	using ResultType = T;

private:
	ContinuousTask<T> CTask;
	MyCoroutine Coroutine;
	std::future<T> Coroutine_Return;
		
	Task(const Task&) = delete;

public:
	Task()
	{
	}

	Task(promise_type& Await)
		: Coroutine(MyCoroutine::from_promise(Await))
	{
		Coroutine_Return = Await.Promise.get_future();
	}

	Task(ContinuousTask<T> CTask)
		: CTask(std::move(CTask))
	{
	}

	Task(Task&& Rhs) noexcept
		: CTask(std::move(Rhs.CTask))
		, Coroutine(std::move(Rhs.Coroutine))
		, Coroutine_Return(std::move(Rhs.Coroutine_Return))
	{
	}

private:
	std::promise<T> Resume_Promise;
	std::future<T> Resume;

public:
	[[nodiscard]]
	bool await_ready() const
	{
		return false;
	}

	template<class U>
	void await_suspend(std::coroutine_handle<AwaitableTask<U, Task>> Coroutine)
	{
		Resume_Promise = {};
		Resume = Resume_Promise.get_future();

		if constexpr (std::same_as<T, void>)
		{
			auto CompletionTask = [this, Coroutine]()
			{
				Resume_Promise.set_value();
				Coroutine.resume();
			};

			if (CTask)
			{
				AwaitableTask<U, Task>& Task = Coroutine.promise();
				Task.Consume(std::move(CTask), CTask.Then<void>(CompletionTask));
			}
			else
			{
				AwaitableTask<T, Task>& Task = this->Coroutine.promise();
				Task.CompletionTask = CompletionTask;
			}
		}
		else
		{
			auto CompletionTask = [this, Coroutine](T ReturnValue)
			{
				Resume_Promise.set_value(ReturnValue);
				Coroutine.resume();
			};

			if (CTask)
			{
				AwaitableTask<U, Task>& Task = Coroutine.promise();
				Task.Consume(std::move(CTask), CTask.Then<void>(CompletionTask));
			}
			else
			{
				AwaitableTask<T, Task>& Task = this->Coroutine.promise();
				Task.CompletionTask = CompletionTask;
			}
		}
	}

	T await_resume()
	{
		return Resume.get();
	}

public:
	[[nodiscard]]
	bool IsValid() const
	{
		if (CTask)
		{
			return true;
		}
		else if (Coroutine_Return.valid())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Wait()
	{
		check(IsValid());

		if (CTask)
		{
			CTask.Wait();
		}
		else if (Coroutine_Return.valid())
		{
			Coroutine_Return.wait();
		}
	}

	[[nodiscard]]
	T GetResult()
	{
		if (CTask)
		{
			return CTask.GetResult();
		}
		else
		{
			return Coroutine_Return.get();
		}
	}

	[[nodiscard]]
	operator T() requires !std::is_same_v<T, void>
	{
		return GetResult();
	}

	[[nodiscard]]
	operator bool() const
	{
		return IsValid();
	}

public:
	[[nodiscard]]
	static Task Run(std::function<T()> Body)
	{
		return ContinuousTask<T>::Run(Body);
	}

	template<class U = void>
	[[nodiscard]]
	ContinuousTask<U> Then(std::function<U(T)> Body)
	{
		return CTask.Then(Body);
	}
};