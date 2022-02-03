// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXDeviceChild.h"

class SDirectXDevice;
class SDirectXFence;
class SDirectXTexture2D;
class SEventHandle;

class DIRECTX_API SDirectXSwapChain : public SDirectXDeviceChild, implements IRHISwapChain
{
	GENERATED_BODY(SDirectXSwapChain)

public:
	ComPtr<IDXGISwapChain4> pSwapChain;
	SPROPERTY(Buffers)
	std::vector<SDirectXTexture2D*> Buffers;

	HANDLE hFenceEvent = nullptr;
	ComPtr<ID3D12Fence> pFence;
	uint64 FenceValue = 0;

public:
	SDirectXSwapChain(SDirectXDevice* Owner, ComPtr<IDXGISwapChain4> pSwapChain, size_t BufferCount);

	using Super::Dispose;

	virtual bool IsReady() override;
	virtual void Wait() override;

	virtual void ResizeBuffers(const Vector2N& Size) override;
	virtual IRHITexture2D* GetBuffer(int32 Index) override;

protected:
	virtual void Dispose(bool bDisposing) override;
};