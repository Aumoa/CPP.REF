// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIEnums.h"

interface IRHIDevice;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHITexture2D;
interface IRHIDeviceContext;

class GAME_API SSceneRenderTargetInterface : implements SObject
{
	GENERATED_BODY(SSceneRenderTargetInterface)

private:
	SPROPERTY(RTV)
	IRHIRenderTargetView* RTV = nullptr;
	SPROPERTY(DSV)
	IRHIDepthStencilView* DSV = nullptr;
	uint8 bBindResources : 1 = false;
	
public:
	uint8 bMultisampled : 1 = false;

protected:
	ERHIResourceStates InitialState = ERHIResourceStates::CopySource;
	ERHIPixelFormat RenderTextureFormat = ERHIPixelFormat::B8G8R8A8_UNORM;

public:
	SSceneRenderTargetInterface(IRHIDevice* InDevice, int32 InNumRTVs = 1, bool bUseDSV = false);

	void SetRenderTargets(IRHITexture2D* InColorBuf, IRHITexture2D* InDepthStencilBuf, bool bBindResources = false);
	void CleanupRenderTargets();

	virtual void TransitRenderTargetViews(IRHIDeviceContext* DeviceContext, bool bTransitToRender);
	virtual void ClearRenderTargetViews(IRHIDeviceContext* DeviceContext);
	virtual void CopyRenderTargetOutput(IRHIDeviceContext* DeviceContext, IRHITexture2D* ColorOutput);
	virtual IRHITexture2D* GetRenderTexture();

	IRHIRenderTargetView* GetRTV();
	IRHIDepthStencilView* GetDSV();
};