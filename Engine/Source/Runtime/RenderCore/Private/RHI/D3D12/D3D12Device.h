// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/DXGI/DXGIFactoryChild.h"
#include "RHI/IRHIDevice.h"
#include "RHI/DirectXCommon.h"

class SD3D12CommandQueue;
class SD3D12ThreadAllocatorContainer;
class SD3D12ThreadDescriptorHeapContainer;
class SD3D12DescriptorHeap;

class SD3D12Device : public SDXGIFactoryChild, implements IRHIDevice
{
	GENERATED_BODY(SD3D12Device)

private:
	SDXGIFactory* _factory = nullptr;
	ComPtr<ID3D12Device> _device;

	std::mutex _allocatorLock;
	std::map<int64, SD3D12ThreadAllocatorContainer*> _threadAllocators;
	std::mutex _heapsLock;
	std::map<int64, SD3D12ThreadDescriptorHeapContainer*> _threadSrvHeaps;
	std::map<int64, SD3D12ThreadDescriptorHeapContainer*> _threadSamplerHeaps;
	
	uint64 _fenceValue = 0;
	SD3D12CommandQueue* _immCon = nullptr;

public:
	SD3D12Device(SDXGIFactory* factory, ComPtr<ID3D12Device> device);

	virtual IRHIDeviceContext* GetImmediateContext() override;
	virtual IRHITexture2D* CreateTexture2D(const RHITexture2DDesc& desc, const RHISubresourceData* initialData) override;
	virtual IRHIBuffer* CreateBuffer(const RHIBufferDesc& desc, const RHISubresourceData* initialData) override;
	virtual IRHIShader* CompileMaterial(SMaterial* material, SVertexFactory* vertexDeclaration) override;
	virtual IRHIRenderTargetView* CreateRenderTargetView(int32 count) override;
	virtual IRHIDepthStencilView* CreateDepthStencilView(int32 count) override;
	virtual IRHIShaderResourceView* CreateShaderResourceView(int32 count) override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	ID3D12CommandAllocator* GetThreadPrimaryAllocator();
	SD3D12DescriptorHeap* GetThreadPrimarySrvHeap(int32 count);
	SD3D12DescriptorHeap* GetThreadPrimarySamplerHeap(int32 count);
	void MarkPendingAllocatorAndHeaps(uint64 fenceValue);

public:
	DECLARE_GETTER(ID3D12Device, _device);

private:
	void AllocateCommandQueue();
};