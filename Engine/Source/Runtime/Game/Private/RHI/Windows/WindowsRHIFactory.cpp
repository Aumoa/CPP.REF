// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIFactory.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"
#include <d3d12.h>

WindowsRHIFactory::WindowsRHIFactory(bool bEnableDebugLayer)
	: RHIFactory(bEnableDebugLayer)
{
	HR(::CreateDXGIFactory1(IID_PPV_ARGS(&_factory)));
}

WindowsRHIFactory::~WindowsRHIFactory() noexcept
{
}

std::shared_ptr<RHIDevice> WindowsRHIFactory::CreateDevice()
{
	return std::shared_ptr<RHIDevice>(new WindowsRHIDevice(std::static_pointer_cast<WindowsRHIFactory>(shared_from_this())));
}

#endif