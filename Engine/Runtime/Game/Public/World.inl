// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Game
{
	template<class T, class... TArgs> requires Core::TIsAssignable<T*, Framework::AActor*> && Core::THasConstructor<T, TArgs...>
	inline T* World::SpawnActor(TArgs&&... constructor_args)
	{
		return Cast<T>(SpawnActorInternal(NewObject<T>(forward<TArgs>(constructor_args)...)));
	}
}