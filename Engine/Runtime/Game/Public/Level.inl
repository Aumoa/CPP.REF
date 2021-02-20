// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<class T, class... TArgs> requires TIsAssignable<T*, AActor*> && THasConstructor<T, TArgs...>
inline T* Level::SpawnActorPersistent(TArgs&&... constructor_args)
{
	if (world == nullptr)
	{
		return nullptr;
	}

	return world->SpawnActor<T>(constructor_args...);
}