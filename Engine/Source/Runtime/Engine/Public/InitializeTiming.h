// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "InitializeTiming.gen.h"

namespace Ayla
{
	AENUM()
	enum class InitializeTiming
	{
		BeforeSceneLoad,
		AfterSceneLoad
	};
}