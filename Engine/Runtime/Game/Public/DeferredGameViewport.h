// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "GameViewport.h"

class DirectXHDRTarget;

class GAME_API DeferredGameViewport : public GameViewport
{
public:
	using Super = GameViewport;

private:
	TRefPtr<DirectXHDRTarget> hdrTarget;

public:
	DeferredGameViewport(DirectXDeviceBundle* deviceBundle);
	~DeferredGameViewport() override;

	void RenderScene(ID3D12GraphicsCommandList4* inCommandList, Scene* inScene) override;

protected:
	void SetViewportResolution_Internal(int32 x, int32 y) override;
};