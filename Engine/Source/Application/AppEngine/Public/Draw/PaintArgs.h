// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;

class APPENGINE_API PaintArgs
{
public:
	SWidget*	const	Parent;
	float		const	DeltaTime;
	float		const	RenderOpacity;

public:
	PaintArgs(SWidget* InParentWidget, float InDeltaTime, float InRenderOpacity);

	PaintArgs WithNewParent(SWidget* InParentWidget) const;
};