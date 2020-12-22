// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "ActorComponent.h"

namespace SC::Runtime::Game::Components
{
	class GAME_API SceneComponent : public ActorComponent
	{
	public:
		using Super = ActorComponent;
		using This = SceneComponent;

	public:
		SceneComponent();
		~SceneComponent();
	};
}