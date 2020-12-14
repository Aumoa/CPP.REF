// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHIDeviceBundle.h"

#include "Logging/Logger.h"
#include "Logging/LogMacros.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game::D3D12RHI;

RHIDeviceBundle::RHIDeviceBundle()
{
#ifdef _DEBUG
	if (ComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		SE_LOG(LogD3D12RHI, Verbose, "D3D12 debug layer enabled.");
	}
	else
	{
		SE_LOG(LogD3D12RHI, Warning, "Cannot enable D3D12 debug layer.");
	}
#endif
}

RHIDeviceBundle::~RHIDeviceBundle()
{

}

void RHIDeviceBundle::InitializeBundle()
{
	HR(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
}