// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIResource : virtual public Core::Object
	{

	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::RHI::IRHIResource;
#endif