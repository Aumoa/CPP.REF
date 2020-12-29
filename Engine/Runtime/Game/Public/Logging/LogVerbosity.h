// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

namespace SC::Runtime::Game::Logging
{
	enum class ELogVerbosity
	{
		Fatal,
		Error,
		Warning,
		Display,
		Log,
		Verbose,
		VeryVerbose,
	};
}

#ifdef __SC_GLOBAL_NAMESPACE__
using SC::Runtime::Game::Logging::ELogVerbosity;
#endif