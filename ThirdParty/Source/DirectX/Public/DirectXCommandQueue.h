// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXDeviceChild.h"

class DIRECTX_API SDirectXCommandQueue : public SDirectXDeviceChild, implements IRHICommandQueue
{
	GENERATED_BODY(SDirectXCommandQueue)

public:
	ComPtr<ID3D12CommandQueue> pQueue;

public:
	SDirectXCommandQueue(IRHIDevice* Owner, ComPtr<ID3D12CommandQueue> pQueue);

	using Super::Dispose;

	virtual int32 AcquireSwapChainImage(IRHISwapChain* SwapChain) override;
	virtual void Present(IRHISwapChain* SwapChain, int32 BufferIndex) override;
	virtual void Submit(std::span<IRHICommandBuffer* const> CommandBuffers, IRHIFence* Fence) override;

protected:
	virtual void Dispose(bool bDisposing) override;
};