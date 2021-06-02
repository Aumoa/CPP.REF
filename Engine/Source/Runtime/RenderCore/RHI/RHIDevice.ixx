// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ComPtr.h"

export module SC.Runtime.RenderCore:RHIDevice;

import SC.Runtime.Core;
import :LogRHI;
import <dxgi1_6.h>;
import <d3d12.h>;

using enum ELogVerbosity;

/// <summary>
/// Provide interface for control all render devices.
/// </summary>
export class RHIDevice : virtual public Object
{
	const uint8 _bDebug : 1;

	ComPtr<IDXGIFactory2> _factory;
	ComPtr<ID3D12Device> _device;

public:
	/// <summary>
	/// Initialize new <see cref="RHIDevice"/> instance.
	/// </summary>
	RHIDevice(bool bDebug = false) : _bDebug(bDebug)
	{
		InitializeCOM();
		InitializeDXGI();
		InitializeD3D12();
	}

private:
	void InitializeCOM()
	{
		LogSystem::Log(LogRHI, Info, L"----- Initialize COM subsystem.");

		HR(LogRHI, CoInitializeEx(nullptr, COINIT_MULTITHREADED));
		LogSystem::Log(LogRHI, Info, L"COM initialized.");

		LogSystem::Log(LogRHI, Info, L"----- Done!");
	}

	void InitializeDXGI()
	{
		LogSystem::Log(LogRHI, Info, L"----- Initialize DXGI subsystem.");

		uint32 flags = _bDebug ? DXGI_CREATE_FACTORY_DEBUG : 0;
		HR(LogRHI, CreateDXGIFactory2(flags, IID_PPV_ARGS(&_factory)));
		LogSystem::Log(LogRHI, Info, L"DXGI factory created.");

		LogSystem::Log(LogRHI, Info, L"----- Done!");
	}

	void InitializeD3D12()
	{
		LogSystem::Log(LogRHI, Info, L"----- Initialize D3D12 subsystem.");

		HR(LogRHI, ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)));
		LogSystem::Log(LogRHI, Info, L"Direct3D 12 device created with feature level 11_0.");

		LogSystem::Log(LogRHI, Info, L"----- Done!");
	}
};