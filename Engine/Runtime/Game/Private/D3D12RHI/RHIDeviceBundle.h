// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIBundle.h"

#include "D3D12Minimal.h"

namespace SC::Runtime::Game::D3D12RHI
{
	class RHIDeviceBundle : virtual public Core::Object, virtual public RHI::IRHIBundle
	{
	public:
		using Super = Core::Object;
		using This = RHIDeviceBundle;

	private:
		ComPtr<IDXGIFactory> dxgiFactory;
		ComPtr<IDXGIAdapter1> primaryAdapter;

		ComPtr<ID3D12Device> d3d12Device;

	public:
		RHIDeviceBundle();
		~RHIDeviceBundle() override;

		virtual void InitializeBundle();
	};
}