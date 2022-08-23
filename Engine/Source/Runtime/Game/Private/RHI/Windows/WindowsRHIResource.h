// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIResource.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;
class WindowsRHISwapChain;

class WindowsRHIResource : public RHIResource
{
	friend WindowsRHIDevice;
	friend WindowsRHISwapChain;

private:
	ComPtr<ID3D12Resource> _resource;

private:
	// for swap chain buffer.
	WindowsRHIResource(std::shared_ptr<RHIDevice> device);
	WindowsRHIResource(std::shared_ptr<WindowsRHIDevice> device, const RHIHeapProperties& heapProps, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* clearValue);

public:
	virtual ~WindowsRHIResource() noexcept override;

public:
	DECLGET(Resource, _resource);
};

inline ID3D12Resource* WinGetr(RHIResource* resource)
{
	if (resource)
	{
		return static_cast<WindowsRHIResource*>(resource)->GetResource().Get();
	}
	else
	{
		return nullptr;
	}
}

#endif