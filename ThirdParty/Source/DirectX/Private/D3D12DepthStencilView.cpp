// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DepthStencilView.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

GENERATE_BODY(SD3D12DepthStencilView);

SD3D12DepthStencilView::SD3D12DepthStencilView(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12DescriptorHeap> heap, size_t resources) : Super(InFactory, InDevice, std::move(heap), resources, D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
{
}

void SD3D12DepthStencilView::CreateDepthStencilView(int32 index, IRHIResource* resource, const RHIDepthStencilViewDesc* desc)
{
	auto Dev = Device->Get<ID3D12Device>();
	Dev->CreateDepthStencilView(
		Cast<SD3D12Resource>(resource)->Get<ID3D12Resource>(),
		reinterpret_cast<const D3D12_DEPTH_STENCIL_VIEW_DESC*>(desc),
		GetHandle(index));

	AssignResource(index, resource);
}