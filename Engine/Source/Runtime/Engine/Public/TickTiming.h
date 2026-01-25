// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickTiming.gen.h"

namespace Ayla
{
	AENUM()
	enum class TickTiming
	{
		Initialization,
		TimeUpdate,
		PreUpdate,
		Update,
		PostUpdate,
		Physics,
		PreLateUpdate,
		LateUpdate,
		PostLateUpdate,
		PreRender,
		PostRender,
		EndOfFrame
	};
}