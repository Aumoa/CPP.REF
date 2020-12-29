// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Game
{
	template<class T, class... TArgs> requires Core::TIsAssignable<T*, Framework::AActor*> && Core::THasConstructor<T, TArgs...>
	inline T* Level::SpawnActorPersistent(TArgs&&... constructor_args)
	{
		if (world == nullptr)
		{
			return nullptr;
		}

		return world->SpawnActor<T>(constructor_args...);
	}
}