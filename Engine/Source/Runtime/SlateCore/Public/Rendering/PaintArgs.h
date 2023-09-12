// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;

struct SLATECORE_API NPaintArgs
{
	const SWidget&	OwningWidget;
	const SWidget*	ParentWidget;
	const TimeSpan	DeltaTime;
	const float		ParentRenderOpacity;

	static NPaintArgs InitPaintArgs(const SWidget& InOwningWidget, const TimeSpan& InDeltaTime);
	NPaintArgs WithNewParent(const SWidget& InOwningWidget) const;
};