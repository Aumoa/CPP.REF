// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12RenderTargetView.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

SD3D12RenderTargetView::SD3D12RenderTargetView(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources) : Super(factory, device, std::move(heap), resources, D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
{
}

void SD3D12RenderTargetView::CreateRenderTargetView(int32 index, IRHIResource* resource, const RHIRenderTargetViewDesc* desc)
{
	auto dev = _device->Get<ID3D12Device>();
	dev->CreateRenderTargetView(
		Cast<SD3D12Resource>(resource)->Get<ID3D12Resource>(),
		reinterpret_cast<const D3D12_RENDER_TARGET_VIEW_DESC*>(desc),
		GetHandle(index));
}