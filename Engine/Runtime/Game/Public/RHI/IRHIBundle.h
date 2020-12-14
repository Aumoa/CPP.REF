// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIBundle : virtual public Core::Object
	{
		virtual void InitializeBundle() = 0;
	};
}