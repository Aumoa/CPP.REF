// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game
{
	interface IEngineTick : virtual public Core::Object
	{
		virtual void Initialize() = 0;
		virtual void PostInitialize() = 0;
		virtual void Tick() = 0;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::IEngineTick;
#endif