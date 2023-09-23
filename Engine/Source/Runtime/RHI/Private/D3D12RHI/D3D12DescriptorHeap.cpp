// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12DescriptorHeap.h"

#if PLATFORM_WINDOWS

ND3D12DescriptorHeap::ND3D12DescriptorHeap(ID3D12Device& InDevice) : pDevice(&InDevice)
{
}

void ND3D12DescriptorHeap::Clear(bool bShrink)
{
	// D3D12 descriptors need not actually unbound.
	Count = 0;

	if (bShrink)
	{
		pShaderVisibleHeap.Reset();
	}

	bDirty = false;
}

void ND3D12DescriptorHeap::Reserve(size_t Count)
{
	if (Count > Capacity)
	{
		Capacity = Count;
		AllocateShaderVisibleHeap();
	}
}

void ND3D12DescriptorHeap::AllocateShaderVisibleHeap()
{
	bDirty = Count != 0;
	
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		.NumDescriptors = (UINT)Capacity,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	};

	HR(pDevice->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&pShaderVisibleHeap)));
}

#endif