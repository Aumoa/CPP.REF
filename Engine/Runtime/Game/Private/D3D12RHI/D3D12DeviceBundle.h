// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIDeviceBundle.h"

#include "D3D12Minimal.h"

namespace SC::Runtime::Game::D3D12RHI
{
	class D3D12SwapChain;
	class D3D12ImmediateCommandList;
	class D3D12OfflineDescriptorManager;

	class D3D12DeviceBundle : virtual public Core::Object, virtual public RHI::IRHIDeviceBundle
	{
	public:
		using Super = Core::Object;
		using This = D3D12DeviceBundle;

	private:
		static D3D12DeviceBundle* instance;

		ComPtr<IDXGIFactory2> dxgiFactory;

		ComPtr<IDXGIAdapter1> primaryAdapter;
		ComPtr<ID3D12Device> d3d12Device;

		Core::TRefPtr<D3D12SwapChain> swapChain;
		Core::TRefPtr<D3D12ImmediateCommandList> immediateCommandList;
		Core::TRefPtr<D3D12OfflineDescriptorManager> rtvManager;
		Core::TRefPtr<D3D12OfflineDescriptorManager> dsvManager;

	public:
		D3D12DeviceBundle();
		~D3D12DeviceBundle() override;

		virtual void InitializeBundle();
		virtual void ReleaseBundle();

		virtual Core::TRefPtr<RHI::IRHISwapChain> GetSwapChain() const;
		virtual Core::TRefPtr<RHI::IRHIImmediateCommandList> GetImmediateCommandList() const;

		virtual Core::TRefPtr<RHI::IRHICommandFence> CreateCommandFence();
		virtual Core::TRefPtr<RHI::IRHIRenderTargetView> CreateRenderTargetView(RHI::IRHIResource* resource);
		virtual Core::TRefPtr<RHI::IRHIResource> CreateTexture2D(RHI::RHITextureFormat format, int32 width, int32 height, RHI::RHIResourceStates initialStates, RHI::RHIResourceFlags flags);
		virtual Core::TRefPtr<RHI::IRHIDeferredCommandList> CreateDeferredCommandList();

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
}