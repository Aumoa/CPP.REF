// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIFactory.h"
#include "RHI/Null/NullRHIFactory.h"

#if PLATFORM_WINDOWS
#include "RHI/Windows/WindowsRHIFactory.h"
#endif

RHIFactory::RHIFactory(bool bEnableDebugLayer)
	: _bEnableDebugLayer(bEnableDebugLayer)
{
}

RHIFactory::~RHIFactory() noexcept
{
}

bool RHIFactory::IsDebugLayerEnabled() const
{
	return _bEnableDebugLayer;
}

std::shared_ptr<RHIFactory> RHIFactory::CreateFactory()
{
	ERHIFactoryLevel level;

#if DO_CHECK
	level = ERHIFactoryLevel::EnableDebugLayer;
#else
	level = ERHIFactoryLevel::Default;
#endif

	if (level == ERHIFactoryLevel::Null)
	{
		return std::shared_ptr<RHIFactory>(new NullRHIFactory());
	}
	else
	{
		bool bEnableDebugLayer = false;
#if !SHIPPING
		bEnableDebugLayer = level == ERHIFactoryLevel::EnableDebugLayer;;
#endif

#if PLATFORM_WINDOWS
		return std::shared_ptr<WindowsRHIFactory>(new WindowsRHIFactory(bEnableDebugLayer));
#else
#endif
	}
}