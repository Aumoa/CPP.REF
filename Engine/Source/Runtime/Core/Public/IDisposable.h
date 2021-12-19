// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "GC/SharedPtr.h"

interface IDisposable : implements SObject
{
	GENERATED_INTERFACE_BODY(IDisposable)

	virtual void Dispose() = 0;
};

namespace IDisposable_Details
{
	template<class T>
	class ScopedAutoDispose : public SharedPtr<T>
	{
		static_assert(std::derived_from<T, IDisposable>);

	public:
		ScopedAutoDispose(T* Disposable) : SharedPtr<T>(Disposable)
		{
		}

		~ScopedAutoDispose() noexcept
		{
			SharedPtr<T>::Get()->Dispose();
		}
	};

	class ScopedUsingExecutor
	{
	public:
		template<class T>
		auto operator <<(T&& InlineLambda)
		{
			return InlineLambda();
		}
	};
}

#define dp_using(Var, Exp) IDisposable_Details::ScopedUsingExecutor() \
<< [&, Var = IDisposable_Details::ScopedAutoDispose(Exp)]() -> void