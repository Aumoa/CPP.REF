// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDevice;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHITexture2D;
interface IRHIDeviceContext;

class RENDERCORE_API SSceneRenderTargetInterface : implements SObject
{
	GENERATED_BODY(SSceneRenderTargetInterface)

private:
	IRHIRenderTargetView* RTV = nullptr;
	IRHIDepthStencilView* DSV = nullptr;
	bool bBindResources = false;

public:
	SSceneRenderTargetInterface(IRHIDevice* InDevice, int32 InNumRTVs = 1, bool bUseDSV = false);

	void SetRenderTargets(IRHITexture2D* InColorBuf, IRHITexture2D* InDepthStencilBuf, bool bBindResources = false);
	void CleanupRenderTargets();

	virtual void CopyRenderTargetOutput(IRHIDeviceContext* DeviceContext, IRHITexture2D* ColorOutput);
	virtual IRHITexture2D* GetRenderTexture();

	IRHIRenderTargetView* GetRTV();
	IRHIDepthStencilView* GetDSV();
};