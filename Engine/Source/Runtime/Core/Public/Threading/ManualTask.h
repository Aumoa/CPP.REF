// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomTaskBase.h"

template<class T = void>
class ManualTask : public CustomTaskBase<T>
{
	using MyAwaiter = typename CustomTaskBase<T>::MyAwaiter;

public:
	ManualTask() : CustomTaskBase<T>(std::make_shared<MyAwaiter>())
	{
	}

	ManualTask(ManualTask&& Rhs) : CustomTaskBase<T>(std::move(Rhs))
	{
	}

	template<class U>
	void SetValue(U&& Value)
	{
		this->GetMyAwaiter()->SetValue(std::forward<U>(Value));
	}

public:
	static Task<> CompletedTask() requires std::same_as<T, void>
	{
		ManualTask<> Task;
		Task.SetValue();
		return Task;
	}

	template<class U>
	static Task<T> CompletedTask(U&& Value) requires (!std::same_as<T, void>)
	{
		ManualTask<T> Task;
		Task.SetValue(std::forward<U>(Value));
		return Task;
	}
};