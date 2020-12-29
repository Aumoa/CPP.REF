// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	enum class RHIResourceStates
	{
		PRESENT,
		RENDER_TARGET,
		COPY_SOURCE,
		COPY_DEST
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::RHI::RHIResourceStates;
#endif