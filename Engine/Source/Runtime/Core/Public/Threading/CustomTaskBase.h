// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "AwaiterBase.h"
#include "Task.h"
#include <memory>

template<class T>
class CustomTaskBase
{
	template<class U>
	friend class CustomTaskBase;

	CustomTaskBase(const CustomTaskBase&) = delete;
	CustomTaskBase& operator =(const CustomTaskBase&) = delete;

protected:
	class MyAwaiter : public Threading::Tasks::AwaiterBase<T>
	{
	};

private:
	std::shared_ptr<MyAwaiter> Awaiter;

public:
	CustomTaskBase()
	{
	}

	CustomTaskBase(CustomTaskBase&& Rhs) : Awaiter(std::move(Rhs.Awaiter))
	{
	}

	bool IsValid() const
	{
		return (bool)Awaiter;
	}

	void Wait()
	{
		Awaiter->Wait();
	}

	T GetValue()
	{
		return Awaiter->GetValue();
	}

	std::shared_ptr<IAwaitable<T>> GetAwaiter() const
	{
		return Awaiter;
	}

	CustomTaskBase& operator =(CustomTaskBase&& Rhs)
	{
		Awaiter = std::move(Rhs.Awaiter);
		return *this;
	}

protected:
	CustomTaskBase(std::shared_ptr<MyAwaiter> Awaiter) : Awaiter(std::move(Awaiter))
	{
	}

public:
	template<class _Fn>
	auto Then(_Fn&& Body)
	{
		return Task<T>(GetAwaiter()).Then(std::forward<_Fn>(Body));
	}
};