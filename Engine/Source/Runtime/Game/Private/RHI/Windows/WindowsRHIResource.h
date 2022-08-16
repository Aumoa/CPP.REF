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

public:
	DECLGET(Resource, _resource);
};

template<class T>
inline ID3D12Resource* WinGetr(const std::shared_ptr<T>& resource) requires std::derived_from<T, RHIResource>
{
	if (resource)
	{
		return CastChecked<WindowsRHIResource>(resource)->GetResource().Get();
	}
	else
	{
		return nullptr;
	}
}

#endif