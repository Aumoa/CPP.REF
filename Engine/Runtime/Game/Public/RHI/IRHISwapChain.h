// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHISwapChain : virtual public Core::Object
	{
		virtual bool Present() = 0;
		virtual bool PresentTest() = 0;
		virtual int32 CurrentBackBufferIndex_get() const = 0;

		vs_property_get(int32, CurrentBackBufferIndex);
	};
}