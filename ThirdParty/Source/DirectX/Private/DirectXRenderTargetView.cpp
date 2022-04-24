// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXRenderTargetView.h"
#include "DirectXDevice.h"
#include "DirectXResource.h"

using namespace ::libty;

SDirectXRenderTargetView::SDirectXRenderTargetView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews)
	: Super(Owner, std::move(pDescriptorHeap), NumViews)
{
}

void SDirectXRenderTargetView::CreateRenderTargetView(size_t index, IRHIResource* pResource, const RHIRenderTargetViewDesc* pDesc)
{
	auto* sDevice = Cast<SDirectXDevice>(GetDevice());
	auto* sResource = Cast<SDirectXResource>(pResource);

	ID3D12Device* lpDevice = sDevice->pDevice.Get();
	ID3D12Resource* lpResource = sResource ? sResource->pResource.Get() : nullptr;

	std::optional<D3D12_RENDER_TARGET_VIEW_DESC> RTVDesc;
	if (pDesc)
	{
		memcpy(&RTVDesc.emplace(), pDesc, sizeof(RHIRenderTargetViewDesc));
	}

	lpDevice->CreateRenderTargetView(lpResource, RTVDesc ? &*RTVDesc : nullptr, GetCPUHandle(index));
	BindResources[index] = pResource;
}