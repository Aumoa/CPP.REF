// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<TIsBaseOf<AActor> T, class... TArgs> requires THasConstructor<T, TArgs...>
inline T* World::SpawnActor(TArgs&&... constructor_args)
{
	return Cast<T>(SpawnActorInternal(NewObject<T>(forward<TArgs>(constructor_args)...)));
}

template<TIsBaseOf<AActor> T>
inline T* World::SpawnActor(TSubclassOf<T> static_class)
{
	return Cast<T>(SpawnActorInternal(static_class.Instantiate()));
}