// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "World.h"

namespace SC::Runtime::Game::Framework
{
	class AActor;
}

namespace SC::Runtime::Game
{
	class World;

	class GAME_API Level : virtual public Core::Object
	{
		friend class World;

	public:
		using Super = Core::Object;
		using This = Level;

	private:
		World* world;

	public:
		Level();
		~Level() override;

		virtual void LoadLevel();

	protected:
		template<class T, class... TArgs> requires Core::TIsAssignable<T*, Framework::AActor*> && Core::THasConstructor<T, TArgs...>
		inline T* SpawnActorPersistent(TArgs&&... constructor_args);

	private:
		void UnloadLevel();
	};
}

#include "Level.inl"

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Level;
#endif