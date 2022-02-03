// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDevice;
class SWidget;
class SceneRenderTarget;

class GAME_API PaintArgs
{
public:
	IRHIDevice* const Device;
	SWidget*	const Parent;
	float		const DeltaTime;
	float		const RenderOpacity;

private:
	PaintArgs(IRHIDevice* Device, SWidget* InParentWidget, float InDeltaTime, float RenderOpacity = 1.0f);

public:
	static PaintArgs InitPaintArgs(IRHIDevice* Device, float InDeltaTime);
	PaintArgs WithNewParent(SWidget* InParentWidget) const;
};