// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDeviceContext;
class SceneRenderTarget;

class GAME_API SceneRenderContext
{
	GENERATED_BODY(SceneRenderContext)

public:
	IRHIDeviceContext* const DeviceContext;
	SceneRenderTarget* const RenderTarget;

public:
	SceneRenderContext(IRHIDeviceContext* InDeviceContext, SceneRenderTarget* InRenderTarget)
		: DeviceContext(InDeviceContext)
		, RenderTarget(InRenderTarget)
	{
	}
};