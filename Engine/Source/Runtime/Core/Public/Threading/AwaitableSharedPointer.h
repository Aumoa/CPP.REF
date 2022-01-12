// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Awaiter.h"

namespace Threading::Tasks
{
	template<class T>
	class AwaitableSharedPointer
	{
		std::shared_ptr<Awaiter<T>> Ptr;

	public:
		AwaitableSharedPointer(std::shared_ptr<Awaiter<T>> Ptr) : Ptr(std::move(Ptr))
		{
		}

		bool await_ready()
		{
			return Ptr->await_ready();
		}

		template<class TCoroutine>
		void await_suspend(TCoroutine Coroutine) const noexcept
		{
			Ptr->await_suspend(std::move(Coroutine));
		}

		T await_resume() const noexcept
		{
			return Ptr->await_resume();
		}

		Awaiter<T>* operator ->() const
		{
			return Ptr.get();
		}
	};
}