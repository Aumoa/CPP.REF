// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIDeviceBundle.h"

#include "D3D12Minimal.h"

class D3D12SwapChain;
class D3D12ImmediateCommandList;
class D3D12OfflineDescriptorManager;

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

public:
	D3D12DeviceBundle();
	~D3D12DeviceBundle() override;

	virtual void InitializeBundle();
	virtual void ReleaseBundle();

	virtual TRefPtr<IRHISwapChain> GetSwapChain() const;
	virtual TRefPtr<IRHIImmediateCommandList> GetImmediateCommandList() const;

	virtual TRefPtr<IRHICommandFence> CreateCommandFence();
	virtual TRefPtr<IRHIRenderTargetView> CreateRenderTargetView(IRHIResource* resource);
	virtual TRefPtr<IRHIResource> CreateTexture2D(RHITextureFormat format, int32 width, int32 height, RHIResourceStates initialStates, RHIResourceFlags flags);
	virtual TRefPtr<IRHIDeferredCommandList> CreateDeferredCommandList();

	vs_property_get(ID3D12Device*, Device);
	ID3D12Device* Device_get() const;

private:
	void InitializeCOM();
	void InitializeDXGI();
	void InitializeD3D12();

	bool IsAdapterSuitable(IDXGIAdapter1* adapter) const;
	bool IsDeviceSuitable(ID3D12Device* device) const;

	// CALLBACK HANDLERS
	void Application_OnSizing(int32 width, int32 height);
};