// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<class T, class... TArgs> requires TIsAssignable<T*, AActor*> && THasConstructor<T, TArgs...>
inline T* GLevel::SpawnActorPersistent(TArgs&&... constructor_args)
{
	if (GetWorld() == nullptr)
	{
		return nullptr;
	}

	return GetWorld()->SpawnActor<T>(constructor_args...);
}