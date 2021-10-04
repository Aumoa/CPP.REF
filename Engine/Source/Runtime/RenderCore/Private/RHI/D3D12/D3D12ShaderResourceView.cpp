// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12ShaderResourceView.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

SD3D12ShaderResourceView::SD3D12ShaderResourceView(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources) : Super(factory, device, std::move(heap), resources, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
{
}

void SD3D12ShaderResourceView::CreateShaderResourceView(int32 index, IRHIResource* resource, const RHIShaderResourceViewDesc* desc)
{
	auto dev = _device->Get<ID3D12Device>();
	dev->CreateShaderResourceView(
		Cast<SD3D12Resource>(resource)->Get<ID3D12Resource>(),
		reinterpret_cast<const D3D12_SHADER_RESOURCE_VIEW_DESC*>(desc),
		GetHandle(index));
}