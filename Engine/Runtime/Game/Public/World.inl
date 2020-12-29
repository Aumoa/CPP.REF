// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

template<class T, class... TArgs> requires TIsAssignable<T*, AActor*> && THasConstructor<T, TArgs...>
inline T* World::SpawnActor(TArgs&&... constructor_args)
{
	return Cast<T>(SpawnActorInternal(NewObject<T>(forward<TArgs>(constructor_args)...)));
}