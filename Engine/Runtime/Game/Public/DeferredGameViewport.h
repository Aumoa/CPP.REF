// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "GameViewport.h"

#include "RHI/RHIViewport.h"

interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHIShaderResourceView;

class GAME_API DeferredGameViewport : public GameViewport
{
public:
	using Super = GameViewport;
	using This = DeferredGameViewport;

private:
	RHIViewport viewport;
	Rect scissor;

	TRefPtr<IRHIResource> renderTarget;
	TRefPtr<IRHIResource> normalBuffer;
	TRefPtr<IRHIResource> depthStencilBuffer;

	TRefPtr<IRHIRenderTargetView> renderTargetView;
	TRefPtr<IRHIRenderTargetView> normalBufferView;
	TRefPtr<IRHIDepthStencilView> depthStencilView;

	TRefPtr<IRHIResource> hdrBuffer;
	TRefPtr<IRHIRenderTargetView> hdrTargetView;
	TRefPtr<IRHIShaderResourceView> colorBufferSRV;
	TRefPtr<IRHIShaderResourceView> normalBufferSRV;
	TRefPtr<IRHIShaderResourceView> hdrBufferSRV;

public:
	DeferredGameViewport();
	~DeferredGameViewport() override;

	void RenderScene(IRHICommandList* inCommandList, Scene* inScene) override;
	IRHIResource* GetRenderTarget() const override;

protected:
	void SetViewportResolution_Internal(int32 x, int32 y) override;

private:
	void BeginGeometryRender(IRHICommandList* inCommandList);
	void EndGeometryRender(IRHICommandList* inCommandList);

	void LightRender(IRHICommandList* inCommandList);
	void TonemapRender(IRHICommandList* inCommandList);
};