// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;

struct SLATECORE_API NPaintArgs
{
	const SWidget&	OwningWidget;
	const SWidget*	ParentWidget;
	const float		DeltaTime;
	const float		ParentRenderOpacity;

	static NPaintArgs InitPaintArgs(const SWidget& InOwningWidget, float InDeltaTime);
	NPaintArgs WithNewParent(const SWidget& InOwningWidget) const;
};