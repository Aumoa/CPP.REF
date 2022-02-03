// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Awaiter.h"
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
	using MyAwaiter = Threading::Tasks::Awaiter<T>;

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

	Threading::Tasks::EStatus GetStatus() const
	{
		return Awaiter->GetStatus();
	}

	std::shared_ptr<MyAwaiter> GetAwaiter() const
	{
		return Awaiter;
	}

	bool Cancel()
	{
		return Awaiter->Cancel();
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

	MyAwaiter* GetMyAwaiter() const
	{
		return Awaiter.get();
	}

public:
	template<class _Fn>
	auto Then(_Fn&& Body)
	{
		return Task<T>(Awaiter).Then(std::forward<_Fn>(Body));
	}

	template<class _Fn>
	void Else(_Fn&& Body)
	{
		Awaiter->Else(std::forward<_Fn>(Body));
	}
};