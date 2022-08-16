// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIResource.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"

std::shared_ptr<RHIResource> WindowsRHIDevice::CreateCommittedResource(const RHIHeapProperties& heapProps, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* clearValue)
{
	return MakeShared<WindowsRHIResource>(SharedFromThis(), heapProps, heapFlags, desc, initialState, clearValue);
}

WindowsRHIResource::WindowsRHIResource(std::shared_ptr<WindowsRHIDevice> device, const RHIHeapProperties& heapProps, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* clearValue)
	: RHIResource(device)
{
	auto* d3ddev = WinGetr(device);
	
	auto d3dHeapFlags = Convert(heapProps);
	auto d3ddesc = Convert(desc);
	D3D12_CLEAR_VALUE d3dClearValue;
	if (clearValue)
	{
		d3dClearValue = Convert(*clearValue);
	}

	HR(d3ddev->CreateCommittedResource(&d3dHeapFlags, Convert(heapFlags), &d3ddesc, Convert(initialState), clearValue ? &d3dClearValue : nullptr, IID_PPV_ARGS(&_resource)));
}

WindowsRHIResource::~WindowsRHIResource() noexcept
{
}

#endif