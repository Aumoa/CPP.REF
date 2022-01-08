// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IAwaitable.h"

namespace Threading::Tasks
{
	template<class T>
	class AwaitableSharedPointer
	{
		std::shared_ptr<IAwaitable<T>> Ptr;

	public:
		AwaitableSharedPointer(std::shared_ptr<IAwaitable<T>> Ptr) : Ptr(std::move(Ptr))
		{
		}

		bool await_ready()
		{
			return Ptr->await_ready();
		}

		void await_suspend(std::coroutine_handle<> Coroutine) const noexcept
		{
			Ptr->await_suspend(std::move(Coroutine));
		}

		T await_resume() const noexcept
		{
			return Ptr->await_resume();
		}
	};
}