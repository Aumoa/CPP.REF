// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DepthStencilView.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

SD3D12DepthStencilView::SD3D12DepthStencilView(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources) : Super(factory, device, std::move(heap), resources, D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
{
}

void SD3D12DepthStencilView::CreateDepthStencilView(int32 index, IRHIResource* resource, const RHIDepthStencilViewDesc* desc)
{
	auto dev = _device->Get<ID3D12Device>();
	dev->CreateDepthStencilView(
		Cast<SD3D12Resource>(resource)->Get<ID3D12Resource>(),
		reinterpret_cast<const D3D12_DEPTH_STENCIL_VIEW_DESC*>(desc),
		GetHandle(index));

	AssignResource(index, resource);
}