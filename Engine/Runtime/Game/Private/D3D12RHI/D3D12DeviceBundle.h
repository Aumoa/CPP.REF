// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIDeviceBundle.h"

#include <span>
#include "D3D12Minimal.h"
#include "RHI/RHIResourceFlags.h"

class D3D12SwapChain;
class D3D12ImmediateCommandList;
class D3D12OfflineDescriptorManager;
class D3D12OnlineDescriptorManager;
class D3D12DynamicBufferManager;

class D3D12DeviceBundle : virtual public Object, virtual public IRHIDeviceBundle
{
public:
	using Super = Object;
	using This = D3D12DeviceBundle;

private:
	static D3D12DeviceBundle* instance;

	ComPtr<IDXGIFactory2> dxgiFactory;

	ComPtr<IDXGIAdapter1> primaryAdapter;
	ComPtr<ID3D12Device> d3d12Device;

	TRefPtr<D3D12SwapChain> swapChain;
	TRefPtr<D3D12ImmediateCommandList> immediateCommandList;
	TRefPtr<D3D12OfflineDescriptorManager> rtvManager;
	TRefPtr<D3D12OfflineDescriptorManager> dsvManager;
	TRefPtr<D3D12OfflineDescriptorManager> srvManager;

	TRefPtr<RHIShaderLibrary> shaderLibrary;
	TRefPtr<RHIResourceGC> resourceGC;

	std::map<size_t, TRefPtr<D3D12DynamicBufferManager>> bufferManager;

public:
	D3D12DeviceBundle();
	~D3D12DeviceBundle() override;

	virtual void InitializeBundle();
	virtual void ReleaseBundle();
	virtual void Commit(IRHICommandList* inCommandList);

	virtual IRHISwapChain* GetSwapChain() const;
	virtual IRHIImmediateCommandList* GetImmediateCommandList() const;
	virtual RHIShaderLibrary* GetShaderLibrary() const;
	virtual RHIResourceGC* GetResourceGC() const;

	virtual TRefPtr<IRHICommandFence> CreateCommandFence();
	virtual TRefPtr<IRHIRenderTargetView> CreateRenderTargetView(IRHIResource* resource);
	virtual TRefPtr<IRHIDepthStencilView> CreateDepthStencilView(IRHIResource* resource, ERHITextureFormat inViewFormat);
	virtual TRefPtr<IRHIShaderResourceView> CreateTextureView(IRHIResource* resource, ERHITextureFormat inViewFormat);
	virtual TRefPtr<IRHIResource> CreateTexture2D(ERHITextureFormat format, int32 width, int32 height, ERHIResourceStates initialStates, ERHIResourceFlags flags, const RHITextureClearValue& inClearValue);
	virtual TRefPtr<IRHIDeferredCommandList> CreateDeferredCommandList();
	virtual TRefPtr<IRHIFence> CreateFence();
	virtual TRefPtr<IRHIOnlineDescriptorPatch> CreateOnlineDescriptorPatch();

	virtual TRefPtr<IRHIRenderTarget> CreateGBufferRenderTarget();
	virtual TRefPtr<IRHIResource> CreateVertexBuffer(std::span<RHIVertex> vertices);
	virtual TRefPtr<IRHIResource> CreateIndexBuffer(std::span<uint32> indices);
	virtual TRefPtr<IRHIResource> CreateDynamicBuffer(size_t sizeInBytes);
	virtual TRefPtr<IRHIResource> CreateImmutableBuffer(size_t sizeInBytes, ERHIResourceStates initialState);

	vs_property_get(ID3D12Device*, Device);
	ID3D12Device* Device_get() const;

private:
	void InitializeCOM();
	void InitializeDXGI();
	void InitializeD3D12();
	void InitializeShaders();

	bool IsAdapterSuitable(IDXGIAdapter1* adapter) const;
	bool IsDeviceSuitable(ID3D12Device* device) const;
	
	ComPtr<ID3D12Resource> CreateImmutableBuffer(D3D12_RESOURCE_STATES initialState, std::span<uint8> initialBuffer, ERHIResourceFlags flags = ERHIResourceFlags::None);

	// CALLBACK HANDLERS
	void Application_OnSizing(int32 width, int32 height);
};