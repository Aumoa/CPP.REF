// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12ShaderResourceView.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

GENERATE_BODY(SD3D12ShaderResourceView);

SD3D12ShaderResourceView::SD3D12ShaderResourceView(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12DescriptorHeap> Heap, size_t NumResources) : Super(InFactory, InDevice, std::move(Heap), NumResources, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
{
}

void SD3D12ShaderResourceView::CreateShaderResourceView(int32 index, IRHIResource* resource, const RHIShaderResourceViewDesc* desc)
{
	auto Dev = Device->Get<ID3D12Device>();
	Dev->CreateShaderResourceView(
		Cast<SD3D12Resource>(resource)->Get<ID3D12Resource>(),
		reinterpret_cast<const D3D12_SHADER_RESOURCE_VIEW_DESC*>(desc),
		GetHandle(index));

	AssignResource(index, resource);
}