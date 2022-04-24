// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXDepthStencilView.h"
#include "DirectXDevice.h"
#include "DirectXResource.h"

using namespace ::libty;

SDirectXDepthStencilView::SDirectXDepthStencilView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews)
	: Super(Owner, std::move(pDescriptorHeap), NumViews)
{
}

void SDirectXDepthStencilView::CreateDepthStencilView(size_t index, IRHIResource* pResource, const RHIDepthStencilViewDesc* pDesc)
{
	auto* sDevice = Cast<SDirectXDevice>(GetDevice());
	auto* sResource = Cast<SDirectXResource>(pResource);

	ID3D12Device* lpDevice = sDevice->pDevice.Get();
	ID3D12Resource* lpResource = sResource ? sResource->pResource.Get() : nullptr;

	std::optional<D3D12_DEPTH_STENCIL_VIEW_DESC> DSVDesc;
	if (pDesc)
	{
		memcpy(&DSVDesc.emplace(), pDesc, sizeof(RHIDepthStencilViewDesc));
	}

	lpDevice->CreateDepthStencilView(lpResource, DSVDesc ? &*DSVDesc : nullptr, GetCPUHandle(index));
	BindResources[index] = pResource;
}