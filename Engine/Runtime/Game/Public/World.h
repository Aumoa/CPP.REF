// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game
{
	class GAME_API World : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = World;

	public:
		World();
		~World() override;

		void Tick();
	};
}