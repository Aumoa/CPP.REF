// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIResource.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;

class WindowsRHIResource : public RHIResource
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3D12Resource> _resource;

private:
	WindowsRHIResource(std::shared_ptr<WindowsRHIDevice> device, const RHIHeapProperties& heapProps, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* clearValue);

public:
	virtual ~WindowsRHIResource() noexcept override;
};

#endif