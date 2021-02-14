// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "GameViewport.h"

#include "RHI/RHIViewport.h"

interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHIShaderResourceView;
interface IRHIRenderTarget;

class GAME_API DeferredGameViewport : public GameViewport
{
public:
	using Super = GameViewport;
	using This = DeferredGameViewport;

private:
	RHIViewport viewport;
	Rect scissor;

	TRefPtr<IRHIRenderTarget> gbuffer;

	TRefPtr<IRHIResource> renderTarget;
	TRefPtr<IRHIRenderTargetView> rtv;

	TRefPtr<IRHIResource> hdrBuffer;
	TRefPtr<IRHIRenderTargetView> hdrTargetView;
	TRefPtr<IRHIShaderResourceView> hdrBufferSRV;

public:
	DeferredGameViewport();
	~DeferredGameViewport() override;

	void RenderScene(IRHICommandList* inCommandList, Scene* inScene) override;
	IRHIResource* GetRenderTarget() const override;

protected:
	void SetViewportResolution_Internal(int32 x, int32 y) override;

private:
	void LightRender(IRHICommandList* inCommandList, Scene* inScene);
	void TonemapRender(IRHICommandList* inCommandList);

	void RecreateRenderTarget();
};