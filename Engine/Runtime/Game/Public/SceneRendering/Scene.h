// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::SceneRendering
{
	class SceneRenderer;

	class GAME_API Scene : virtual public Core::Object
	{
	public:
		Scene();
		~Scene() override;

		void Render(SceneRenderer* renderer);
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::SceneRendering::Scene;
#endif