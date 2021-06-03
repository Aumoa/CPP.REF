// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import SC.Runtime.RenderCore.Internal;

using enum ELogVerbosity;

RHIDevice::RHIDevice(bool bDebug) : Super()
	, _bDebug(bDebug)
{
	if (bDebug)
	{
		InitializeDebug();
	}

	InitializeCOM();
	InitializeDXGI();
	InitializeD3D12();
}

RHIDevice::~RHIDevice()
{
}

void RHIDevice::InitializeDebug()
{
	LogSystem::Log(LogRHI, Info, L"----- Initialize Direct3D 12 debug layer.");

	if (ComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
	}
	else
	{
		LogSystem::Log(LogRHI, Warning, L"Direct3D 12 debug layer could not be enabled.");
	}

	LogSystem::Log(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeCOM()
{
	LogSystem::Log(LogRHI, Info, L"----- Initialize COM subsystem.");

	HR(LogRHI, CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED));
	LogSystem::Log(LogRHI, Info, L"COM initialized.");

	LogSystem::Log(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeDXGI()
{
	LogSystem::Log(LogRHI, Info, L"----- Initialize DXGI subsystem.");

	uint32 flags = _bDebug ? DXGI_CREATE_FACTORY_DEBUG : 0;
	HR(LogRHI, CreateDXGIFactory2(flags, IID_PPV_ARGS(&_factory)));
	LogSystem::Log(LogRHI, Info, L"DXGI factory created.");

	LogSystem::Log(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeD3D12()
{
	constexpr double InvGiB = 1.0 / 1024.0 / 1024.0 / 1024.0;

	LogSystem::Log(LogRHI, Info, L"----- Initialize D3D12 subsystem.");

	LogSystem::Log(LogRHI, Info, L"Finding most suitable physical device.");
	ComPtr<IDXGIAdapter1> adapter;
	for (int32 i = 0; SUCCEEDED(_factory->EnumAdapters1((uint32)i, &adapter)); ++i)
	{
		DXGI_ADAPTER_DESC1 desc;
		if (FAILED(adapter->GetDesc1(&desc)))
		{
			adapter.Reset();
			continue;
		}

		if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
		{
			adapter.Reset();
			continue;
		}

		LogSystem::Log(LogRHI, Info, L"Suitable adapter found. Adapter: {}, VideoMemory: {:.2} GiB", desc.Description, desc.DedicatedVideoMemory * InvGiB);
		break;
	}

	if (!adapter.IsSet())
	{
		LogSystem::Log(LogRHI, Fatal, L"Could not find suitable physical device.");
		return;
	}

	HR(LogRHI, D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)));
	LogSystem::Log(LogRHI, Info, L"Direct3D 12 device created with feature level 11_0.");

	LogSystem::Log(LogRHI, Info, L"----- Done!");
}