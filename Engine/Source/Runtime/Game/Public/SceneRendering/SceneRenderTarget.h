// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIStructures.h"

interface IRHITexture2D;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;

class GAME_API SSceneRenderTarget : implements SObject
{
	GENERATED_BODY(SSceneRenderTarget)

public:
	SSceneRenderTarget();
	SSceneRenderTarget(IRHIRenderTargetView* InRTV, int32 IndexOfRTV, IRHIDepthStencilView* InDSV, int32 IndexOfDSV, ERHIResourceStates InInitState);
	~SSceneRenderTarget();

	SPROPERTY(RTV)
	IRHIRenderTargetView* RTV = nullptr;
	SPROPERTY(DSV)
	IRHIDepthStencilView* DSV = nullptr;
	SPROPERTY(RTTexture)
	IRHITexture2D* RTTexture = nullptr;
	SPROPERTY(DSTexture)
	IRHITexture2D* DSTexture = nullptr;
	int32 IndexOfRTV;
	int32 IndexOfDSV;
	ERHIResourceStates InitState;
	RHIViewport Viewport;
	RHIScissorRect ScissorRect;

	uint8 bHasBeenTransited : 1;

	void InitTarget(IRHIRenderTargetView* InRTV, int32 IndexOfRTV, IRHIDepthStencilView* InDSV, int32 IndexOfDSV, ERHIResourceStates InInitState);
};