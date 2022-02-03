// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;
class SSceneRenderTargetInterface;

class SceneRenderContext
{
public:
	IRHIDeviceContext* const DeviceContext;
	SSceneRenderTargetInterface* const RenderTarget;

public:
	SceneRenderContext(IRHIDeviceContext* InDeviceContext, SSceneRenderTargetInterface* InRenderTarget)
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