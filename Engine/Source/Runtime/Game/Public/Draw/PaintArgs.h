// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;
class SceneRenderer;
class SWidget;

class GAME_API PaintArgs
{
public:
	SWidget*			const Parent;
	float				const DeltaTime;

	IRHIDeviceContext*  const DeviceContext;
	SceneRenderer*		const Renderer;

public:
	PaintArgs(SWidget* InParentWidget, float InDeltaTime, IRHIDeviceContext* InDeviceContext, SceneRenderer* Renderer);

	PaintArgs WithNewParent(SWidget* InParentWidget) const;
};