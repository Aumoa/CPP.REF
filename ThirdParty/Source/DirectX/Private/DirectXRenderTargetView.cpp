// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXRenderTargetView.h"
#include "DirectXDevice.h"
#include "DirectXResource.h"

GENERATE_BODY(SDirectXRenderTargetView);

SDirectXRenderTargetView::SDirectXRenderTargetView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews)
	: Super(Owner, std::move(pDescriptorHeap), NumViews)
{
}

void SDirectXRenderTargetView::CreateRenderTargetView(int32 Index, IRHIResource* Resource, const RHIRenderTargetViewDesc* Desc)
{
	auto* sDevice = Cast<SDirectXDevice>(GetDevice());
	auto* sResource = Cast<SDirectXResource>(Resource);

	ID3D12Device* pDevice = sDevice->pDevice.Get();
	ID3D12Resource* pResource = sResource ? sResource->pResource.Get() : nullptr;

	std::optional<D3D12_RENDER_TARGET_VIEW_DESC> RTVDesc;
	if (Desc)
	{
		memcpy(&RTVDesc.emplace(), Desc, sizeof(RHIRenderTargetViewDesc));
	}

	pDevice->CreateRenderTargetView(pResource, RTVDesc ? &*RTVDesc : nullptr, GetCPUHandle(Index));
	BindResources[Index] = Resource;
}