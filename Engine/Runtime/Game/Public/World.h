// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <chrono>

namespace SC::Runtime::Game::Framework
{
	class AActor;
}

namespace SC::Runtime::Game
{
	enum class TickingGroup;
	struct TickFunction;
	class Level;

	class GAME_API World : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = World;

	private:
		std::vector<Core::TRefPtr<Framework::AActor>> actors;
		std::map<TickingGroup, std::vector<TickFunction*>> tickGroups;
		Core::TWeakPtr<Level> currentLevel = nullptr;

	public:
		World();
		~World() override;

		virtual void Tick(std::chrono::duration<double> deltaTime);

		template<class T, class... TArgs> requires Core::TIsAssignable<T*, Framework::AActor*> && Core::THasConstructor<T, TArgs...>
		inline T* SpawnActor(TArgs&&... constructor_args);

		void LoadLevel(Level* loadLevel);

	private:
		void Tick_PrePhysics(std::chrono::duration<double> deltaTime);
		Framework::AActor* SpawnActorInternal(Core::TRefPtr<Framework::AActor> actor);
	};
}

#include "World.inl"

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::World;
#endif