// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIShaderResourceView.h"
#include "InternalComPtr.h"
#include "RHI/LogRHI.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHITexture2D.h"

RHIShaderResourceView::RHIShaderResourceView(RHIDevice* device, uint32 descriptorCount) : Super(device)
	, _descriptorCount(descriptorCount)
{
	ID3D12Device* dev = device->GetDevice();
	D3D12_DESCRIPTOR_HEAP_DESC heapd =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		.NumDescriptors = descriptorCount,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	};
	HR(LogRHI, dev->CreateDescriptorHeap(&heapd, IID_PPV_ARGS(&_descriptor)));
	_increment = dev->GetDescriptorHandleIncrementSize(heapd.Type);
}

RHIShaderResourceView::~RHIShaderResourceView()
{
}

void RHIShaderResourceView::CreateShaderResourceView(RHITexture2D* texture, int32 index)
{
	ID3D12Resource* resource = texture->GetResource();
	ID3D12Device* dev = GetDevice()->GetDevice();

	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descriptor->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (size_t)_increment * index;

	dev->CreateShaderResourceView(resource, nullptr, handle);
}

D3D12_CPU_DESCRIPTOR_HANDLE RHIShaderResourceView::GetCPUDescriptorHandle(int32 index) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descriptor->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (size_t)_increment * index;
	return handle;
}