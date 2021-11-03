// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;
class SSceneRenderTargetInterface;

class SceneRenderContext
{
	GENERATED_BODY(SceneRenderContext)

public:
	IRHIDeviceContext* const DeviceContext;
	SSceneRenderTargetInterface* const RenderTarget;

public:
	SceneRenderContext(IRHIDeviceContext* InDeviceContext, SSceneRenderTargetInterface* InRenderTarget)
		: DeviceContext(InDeviceContext)
		, RenderTarget(InRenderTarget)
	{
	}
};