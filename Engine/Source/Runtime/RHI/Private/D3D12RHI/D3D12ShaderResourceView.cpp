// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12ShaderResourceView.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Texture2D.h"

ND3D12ShaderResourceView::ND3D12ShaderResourceView(ID3D12Device& InDevice, size_t InNumViews)
	: pDevice(&InDevice)
	, NumViews(InNumViews)
{
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		.NumDescriptors = (UINT)NumViews,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	};

	HR(pDevice->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&pHeap)));
}

void ND3D12ShaderResourceView::CreateView(size_t Index, NRHITexture2D* InTextureResource)
{
	if (Index >= NumViews)
	{
		throw ArgumentOutOfRangeException();
	}

	ID3D12Resource* pResource = nullptr;
	DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (InTextureResource)
	{
		pResource = static_cast<ND3D12Texture2D*>(InTextureResource)->GetResource();
		Format = pResource->GetDesc().Format;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc =
	{
		.Format = Format,
		.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D,
		.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
		.Texture2D =
		{
			.MipLevels = 1
		}
	};

	D3D12_CPU_DESCRIPTOR_HANDLE hView = pHeap->GetCPUDescriptorHandleForHeapStart();
	hView.ptr += ((size_t)pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * Index);
	pDevice->CreateShaderResourceView(pResource, &SRVDesc, hView);
}

#endif