// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHISwapChain.h"

class SD3D12Texture2D;

class DIRECTX_API SDXGISwapChain : public SD3D12DeviceChild, implements IRHISwapChain
{
	GENERATED_BODY(SDXGISwapChain)

private:
	ComPtr<IDXGISwapChain4> SwapChain;
	SPROPERTY(Buffers)
	std::array<SD3D12Texture2D*, 3> Buffers = {};

public:
	SDXGISwapChain(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<IDXGISwapChain4> SwapChain);

	virtual void Present(int32 vSyncLevel) override;
	virtual void ResizeBuffers(int32 InWidth, int32 InHeight) override;
	virtual IRHITexture2D* GetBuffer(int32 Index) override;
	virtual int32 GetCurrentBackBufferIndex() override;

protected:
	virtual void Dispose(bool bDisposing);
};