// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXShaderResourceView.h"
#include "DirectXDevice.h"
#include "DirectXResource.h"

GENERATE_BODY(SDirectXShaderResourceView);

SDirectXShaderResourceView::SDirectXShaderResourceView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews)
	: Super(Owner, std::move(pDescriptorHeap), NumViews)
{
}

void SDirectXShaderResourceView::CreateShaderResourceView(size_t index, IRHIResource* pResource, const RHIShaderResourceViewDesc* pDesc)
{
	auto* sDevice = Cast<SDirectXDevice>(GetDevice());
	auto* sResource = Cast<SDirectXResource>(pResource);

	ID3D12Device* lpDevice = sDevice->pDevice.Get();
	ID3D12Resource* lpResource = sResource ? sResource->pResource.Get() : nullptr;

	std::optional<D3D12_SHADER_RESOURCE_VIEW_DESC> SRVDesc;
	if (pDesc)
	{
		memcpy(&SRVDesc.emplace(), pDesc, sizeof(RHIShaderResourceViewDesc));
	}

	lpDevice->CreateShaderResourceView(lpResource, SRVDesc ? &*SRVDesc : nullptr, GetCPUHandle(index));
	BindResources[index] = pResource;
}

void SDirectXShaderResourceView::CreateUnorderedAccessView(size_t index, IRHIResource* pResource, IRHIResource* pCounter, const RHIUnorderedAccessViewDesc* pDesc)
{
	auto* sDevice = Cast<SDirectXDevice>(GetDevice());
	auto* sResource = Cast<SDirectXResource>(pResource);
	auto* sCounter = Cast<SDirectXResource>(pCounter);

	ID3D12Device* lpDevice = sDevice->pDevice.Get();
	ID3D12Resource* lpResource = sResource ? sResource->pResource.Get() : nullptr;
	ID3D12Resource* lpCounter = sCounter ? sCounter->pResource.Get() : nullptr;

	std::optional<D3D12_UNORDERED_ACCESS_VIEW_DESC> UAVDesc;
	if (pDesc)
	{
		memcpy(&UAVDesc.emplace(), pDesc, sizeof(RHIUnorderedAccessViewDesc));
	}

	lpDevice->CreateUnorderedAccessView(lpResource, lpCounter, UAVDesc ? &*UAVDesc : nullptr, GetCPUHandle(index));
	BindResources[index] = pResource;
}