// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDeviceBundle.h"

#include "DirectXCommon.h"
#include "Logging/LogMacros.h"
#include "COM/COMMinimal.h"

DirectXDeviceBundle::DirectXDeviceBundle() : Super()
{
	UINT flag = 0;

#if defined(_DEBUG) && !defined(_DEBUGGAME)
	if (TComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		SE_LOG(LogDirectX, Verbose, "D3D12 debug layer enabled.");
	}
	else
	{
		SE_LOG(LogDirectX, Warning, "Cannot enable D3D12 debug layer.");
	}
	flag |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	// Initialize DirectX Graphics Infrastructure Factory.
	HR(CreateDXGIFactory2(flag, IID_PPV_ARGS(&dxgiFactory)));

	TComPtr<IDXGIAdapter1> adapter;
	for (int32 i = 0; SUCCEEDED(dxgiFactory->EnumAdapters1((UINT)i, &adapter)); ++i)
	{
		if (!IsAdapterSuitable(adapter.Get()))
		{
			continue;
		}

		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device))))
		{
			continue;
		}

		if (!IsDeviceSuitable(device.Get()))
		{
			continue;
		}

		DXGI_ADAPTER_DESC1 desc = { };
		HR(adapter->GetDesc1(&desc));

		SE_LOG(LogDirectX, Verbose, L"That supported feature level 12_1 device named to {0} is selected.", desc.Description);
		break;
	}

	if (!device.IsValid)
	{
		SE_LOG(LogDirectX, Error, "Failed to detect adapter that support feature level 12_1. Create device with software platform.");
		throw COMException(E_NOINTERFACE);
	}
}

DirectXDeviceBundle::~DirectXDeviceBundle()
{

}

IDXGIFactory2* DirectXDeviceBundle::GetFactory() const
{
	return dxgiFactory.Get();
}

ID3D12Device5* DirectXDeviceBundle::GetDevice() const
{
	return device.Get();
}

bool DirectXDeviceBundle::IsAdapterSuitable(IDXGIAdapter1* adapter) const
{
	DXGI_ADAPTER_DESC1 desc = { };
	HR(adapter->GetDesc1(&desc));

	if (desc.Flags != DXGI_ADAPTER_FLAG_NONE) {
		// Is remote or software implement.
		return false;
	}

	return true;
}

bool DirectXDeviceBundle::IsDeviceSuitable(ID3D12Device5* device) const
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options = { };
	HR(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, sizeof(options)));

	// Must support raytracing tier 1.0.
	if (options.RaytracingTier >= D3D12_RAYTRACING_TIER_1_0)
	{
		return true;
	}

	return false;
}