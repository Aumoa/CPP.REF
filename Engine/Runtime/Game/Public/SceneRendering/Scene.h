// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::SceneRendering
{
	class GAME_API Scene : virtual public Core::Object
	{
	public:
		Scene();
		~Scene() override;
	};
}