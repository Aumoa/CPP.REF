// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDevice;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHITexture2D;

class RENDERCORE_API SSceneRenderTarget : implements SObject
{
	GENERATED_BODY(SSceneRenderTarget)

private:
	IRHIRenderTargetView* RTV = nullptr;
	IRHIDepthStencilView* DSV = nullptr;
	bool bBindResources = false;

public:
	SSceneRenderTarget(IRHIDevice* InDevice, int32 InNumRTVs = 1, bool bUseDSV = false);

	void SetRenderTargets(IRHITexture2D* InColorBuf, IRHITexture2D* InDepthStencilBuf, bool bBindResources = false);
	void CleanupRenderTargets();
};