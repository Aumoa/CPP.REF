// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "GameViewport.h"

class DirectXDeviceBundle;
class DirectXHDRTarget;
class DirectXCompatibleRenderTarget;

class GAME_API DeferredGameViewport : public GameViewport
{
public:
	using Super = GameViewport;

private:
	TRefPtr<DirectXHDRTarget> hdrTarget;
	TRefPtr<DirectXCompatibleRenderTarget> compatibleTarget;

public:
	DeferredGameViewport(DirectXDeviceBundle* deviceBundle);
	~DeferredGameViewport() override;

	void RenderScene(IRHICommandList* inCommandList, IRHIScene* inScene) override;
	IRHIResource* GetRenderTarget() const override;

protected:
	void SetViewportResolution_Internal(int32 x, int32 y) override;

private:
	void LightRender(IRHICommandList* inCommandList, IRHIScene* inScene);
	void TonemapRender(IRHICommandList* inCommandList);
};