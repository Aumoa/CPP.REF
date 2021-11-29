// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;
class SWidget;
class SceneRenderTarget;

class SLATECORE_API PaintArgs
{
public:
	SWidget*			const	Parent;
	float				const	DeltaTime;
	float				const	RenderOpacity;

public:
	PaintArgs(SWidget* InParentWidget, float InDeltaTime, float RenderOpacity = 1.0f);

	PaintArgs WithNewParent(SWidget* InParentWidget) const;
};