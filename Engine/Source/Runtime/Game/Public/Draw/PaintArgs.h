// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;
class SWidget;
class SceneRenderTarget;

class GAME_API PaintArgs
{
public:
	SWidget*			const	Parent;
	float				const	DeltaTime;

	IRHIDeviceContext*  const	DeviceContext;
	SceneRenderTarget*	const	RenderTarget;

public:
	PaintArgs(SWidget* InParentWidget, float InDeltaTime, IRHIDeviceContext* InDeviceContext, SceneRenderTarget* RT);

	PaintArgs WithNewParent(SWidget* InParentWidget) const;
	PaintArgs WithNewRenderTarget(SceneRenderTarget* RT) const;
};