// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIStructures.h"

interface IRHITexture2D;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;

class SCENERENDER_API SceneRenderTarget
{
public:
	SceneRenderTarget(IRHIRenderTargetView* InRTV, int32 IndexOfRTV, IRHIDepthStencilView* InDSV, int32 IndexOfDSV, ERHIResourceStates InInitState);

	IRHIRenderTargetView* RTV;
	IRHIDepthStencilView* DSV;
	IRHITexture2D* RTTexture;
	IRHITexture2D* DSTexture;
	int32 IndexOfRTV;
	int32 IndexOfDSV;
	ERHIResourceStates InitState;
	RHIViewport Viewport;
	RHIScissorRect ScissorRect;
};