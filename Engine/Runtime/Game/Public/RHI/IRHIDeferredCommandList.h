// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHICommandList.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIDeferredCommandList : virtual public Core::Object, virtual public IRHICommandList
	{

	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::RHI::IRHIDeferredCommandList;
#endif