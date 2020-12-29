// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <chrono>

namespace SC::Runtime::Game
{
	class World;
	class Level;

	class GAME_API GameInstance : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = GameInstance;

	private:
		static Core::TRefPtr<Core::String> defaultAppName;

		Core::TRefPtr<World> world;

	public:
		GameInstance();
		~GameInstance() override;

		Core::TRefPtr<Core::String> ToString() const override;

		virtual void Initialize();
		virtual void Tick(std::chrono::duration<double> deltaTime);
		virtual void BeginPlay();
		virtual void EndPlay();

		virtual Level* GetStartupLevel() = 0;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::GameInstance;
#endif