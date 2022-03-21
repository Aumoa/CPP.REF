// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
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
	virtual IRHIResource* CreateCommittedResource(const RHIHeapProperties& heapProperties, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* pOptimizedClearValue) override;
	virtual IRHIRenderTargetView* CreateRenderTargetView(size_t count) override;
	virtual IRHIDepthStencilView* CreateDepthStencilView(size_t count) override;
	virtual IRHIShaderResourceView* CreateShaderResourceView(size_t count) override;
	virtual IRHIFence* CreateFence() override;
	virtual IRHICommandAllocator* CreateCommandAllocator(ERHICommandListType type) override;
	virtual IRHIGraphicsCommandList* CreateCommandList(IRHICommandAllocator* pAllocator, ERHICommandListType type, IRHIPipelineState* pInitialState) override;
	virtual IRHIShaderCodeWorkspace* CreateShaderCodeWorkspace() override;
	virtual IRHIRootSignature* CreateRootSignature(const RHIRootSignatureDesc& desc) override;
	virtual IRHIPipelineState* CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& desc) override;

protected:
	virtual void Dispose(bool bDisposing) override;
};