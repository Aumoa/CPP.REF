// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::Framework
{
	class GAME_API AActor : virtual public Core::Object
	{
	public:
		using Super = Core::Object;
		using This = AActor;

	public:
		AActor();
		~AActor() override;
	};
}