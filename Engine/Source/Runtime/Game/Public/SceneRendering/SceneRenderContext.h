// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;
class SSceneRenderTarget;

class SceneRenderContext
{
public:
	IRHIDeviceContext* const DeviceContext;
	SSceneRenderTarget* const RenderTarget;

public:
	SceneRenderContext(IRHIDeviceContext* InDeviceContext, SSceneRenderTarget* InRenderTarget)
		: DeviceContext(InDeviceContext)
		, RenderTarget(InRenderTarget)
	{
	}

	SceneRenderContext(const SceneRenderContext& Context)
		: DeviceContext(Context.DeviceContext)
		, RenderTarget(Context.RenderTarget)
	{
	}
};