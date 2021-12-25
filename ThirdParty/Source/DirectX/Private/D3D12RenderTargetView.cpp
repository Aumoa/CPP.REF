// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12RenderTargetView.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

GENERATE_BODY(SD3D12RenderTargetView);

SD3D12RenderTargetView::SD3D12RenderTargetView(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12DescriptorHeap> Heap, size_t NumResources)
	: Super(InFactory, InDevice, std::move(Heap), NumResources, D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
{
}

void SD3D12RenderTargetView::CreateRenderTargetView(int32 Index, IRHIResource* Resource, const RHIRenderTargetViewDesc* Desc)
{
	auto Dev = Device->Get<ID3D12Device>();
	Dev->CreateRenderTargetView(
		Cast<SD3D12Resource>(Resource)->Get<ID3D12Resource>(),
		reinterpret_cast<const D3D12_RENDER_TARGET_VIEW_DESC*>(Desc),
		GetHandle(Index));

	AssignResource(Index, Resource);
}