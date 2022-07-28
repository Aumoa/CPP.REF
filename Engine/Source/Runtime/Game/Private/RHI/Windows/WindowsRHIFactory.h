// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIFactory.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIFactory : public RHIFactory
{
	friend RHIFactory;

	ComPtr<IDXGIFactory4> _factory;

private:
	WindowsRHIFactory(bool bEnableDebugLayer);

public:
	virtual ~WindowsRHIFactory() noexcept override;

	virtual std::shared_ptr<RHIDevice> CreateDevice() override;

	inline IDXGIFactory4* GetFactory() const { return _factory.Get(); }
};

#endif