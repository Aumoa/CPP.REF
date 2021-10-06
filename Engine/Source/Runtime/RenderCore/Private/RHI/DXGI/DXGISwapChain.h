// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/D3D12/D3D12DeviceChild.h"
#include "RHI/IRHISwapChain.h"
#include "RHI/DirectXCommon.h"

class SD3D12Texture2D;

class SDXGISwapChain : public SD3D12DeviceChild, implements IRHISwapChain
{
	GENERATED_BODY(SDXGISwapChain)

private:
	ComPtr<IDXGISwapChain4> _swapChain;
	std::array<SD3D12Texture2D*, 3> _buffers = {};

public:
	SDXGISwapChain(SDXGIFactory* factory, SD3D12Device* device, ComPtr<IDXGISwapChain4> swapChain);

	virtual void Present(int32 vSyncLevel) override;
	virtual void ResizeBuffers(int32 width, int32 height) override;
	virtual IRHITexture2D* GetBuffer(int32 index) override;
	virtual int32 GetCurrentBackBufferIndex() override;
};