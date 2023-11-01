// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformInputEvent.h"

class SWidget;

struct SLATECORE_API NPaintArgs
{
	const SWidget&	OwningWidget;
	const SWidget*	ParentWidget;
	const TimeSpan	DeltaTime;
	const float		RenderOpacity;

	const std::vector<NGenericPlatformInputEvent>& InputEvents;
	std::vector<bool>& InputEventHandled;

	static NPaintArgs InitPaintArgs(const SWidget& InOwningWidget, const TimeSpan& InDeltaTime, const std::vector<NGenericPlatformInputEvent>& InInputEvents, std::vector<bool>& InInputEventHandled);
	NPaintArgs WithNewParent(const SWidget& InOwningWidget) const;
};