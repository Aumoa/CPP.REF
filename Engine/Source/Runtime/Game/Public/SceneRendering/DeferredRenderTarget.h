// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RenderTarget.h"

class DirectXDeviceBundle;

class GAME_API DeferredRenderTarget : public RenderTarget
{
public:
	using Super = RenderTarget;

private:
	DirectXDeviceBundle* deviceBundle;

public:
	DeferredRenderTarget(DirectXDeviceBundle* deviceBundle);
	~DeferredRenderTarget() override;

	void BeginDraw(ID3D12GraphicsCommandList4* inCommandList) override;
	void EndDraw(ID3D12GraphicsCommandList4* inCommandList) override;
	void ResizeBuffers(uint32 width, uint32 height) override;

private:
	TComPtr<ID3D12Resource> CreateRenderTexture2D(uint32 width, uint32 height);
	TComPtr<ID3D12Resource> CreateDepthStencilTexture2D(uint32 width, uint32 height);
};