// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::Components
{
	class GAME_API ActorComponent : public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = ActorComponent;

	public:
		ActorComponent();
		~ActorComponent() override;
	};
}