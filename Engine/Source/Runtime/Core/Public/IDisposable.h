// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "GC/SharedPtr.h"

interface IDisposable : implements SObject
{
	virtual void Dispose() = 0;
};

namespace IDisposable_Details
{
	template<std::derived_from<IDisposable> T>
	class ScopedAutoDispose : public SharedPtr<T>
	{
	public:
		ScopedAutoDispose(T* Disposable) : SharedPtr(Disposable)
		{
		}

		~ScopedAutoDispose() noexcept
		{
			SharedPtr<T>::Get()->Dispose();
		}
	};

	class ScopedUsingExecutor
	{
		template<class T>
		auto operator <<(T&& InlineLambda)
		{
			return InlineLambda();
		}
	};
}

#define dp_using(Type, Var, Exp) IDisposable_Details::ScopedUsingExecutor() << [Var = ScopedAutoDispose(Exp)]