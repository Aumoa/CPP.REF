// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXFactoryChild.h"

class DIRECTX_API SDirectXDevice : public SDirectXFactoryChild, implements IRHIDevice
{
	GENERATED_BODY(SDirectXDevice)

public:
	ComPtr<ID3D12Device> pDevice;

public:
	SDirectXDevice(SDirectXFactory* Owner, ComPtr<ID3D12Device> pDevice);

	using Super::Dispose;

	virtual IRHICommandQueue* CreateCommandQueue() override;
	virtual IRHITexture2D* CreateTexture2D(const RHITexture2DDesc& InDesc, const RHISubresourceData* InitialData) override { throw; }
	virtual IRHIRenderTargetView* CreateRenderTargetView(int32 Count) override;
	virtual IRHIFence* CreateFence() override;
	virtual IRHICommandAllocator* CreateCommandAllocator() override;
	virtual IRHICommandBuffer* CreateCommandBuffer(IRHICommandAllocator* Allocator) override;
	virtual IRHIRenderPass* CreateRenderPass() override;
	virtual IRHIShaderCodeWorkspace* CreateShaderCodeWorkspace() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};