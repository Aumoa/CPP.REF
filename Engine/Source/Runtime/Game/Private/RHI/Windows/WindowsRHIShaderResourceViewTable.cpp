// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIShaderResourceViewTable.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/Windows/WindowsRHIResource.h"

std::shared_ptr<RHIShaderResourceViewTable> WindowsRHIDevice::CreateShaderResourceViewTable(ERHIShaderResourceViewType type, size_t numViews)
{
	return MakeShared<WindowsRHIShaderResourceViewTable>(SharedFromThis(), type, numViews);
}

WindowsRHIShaderResourceViewTable::WindowsRHIShaderResourceViewTable(std::shared_ptr<WindowsRHIDevice> device, ERHIShaderResourceViewType type, size_t numViews)
	: RHIShaderResourceViewTable(device)
{
	auto* d3ddev = device->GetDevice().Get();

	D3D12_DESCRIPTOR_HEAP_DESC desc =
	{
		.Type = Convert(type),
		.NumDescriptors = (UINT)numViews,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		.NodeMask = 0
	};

	HR(d3ddev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descriptorHeap)));
	_incrementSize = d3ddev->GetDescriptorHandleIncrementSize(desc.Type);
}

WindowsRHIShaderResourceViewTable::~WindowsRHIShaderResourceViewTable() noexcept
{
}

void WindowsRHIShaderResourceViewTable::CreateUnorderedAccessView(size_t viewIndex, std::shared_ptr<RHIResource> pResource, std::shared_ptr<RHIResource> pCounter, const RHIUnorderedAccessViewDesc& desc)
{
	auto* d3ddev = WinGetr(GetDevice());
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _incrementSize * viewIndex;

	d3ddev->CreateUnorderedAccessView(WinGetr(pResource), WinGetr(pCounter), reinterpret_cast<const D3D12_UNORDERED_ACCESS_VIEW_DESC*>(&desc), handle);
}

#endif