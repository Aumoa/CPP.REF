// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIBundle : virtual public Core::Object
	{
		virtual void InitializeBundle() = 0;
		virtual void ReleaseBundle() = 0;
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::RHI::IRHIBundle;
#endif