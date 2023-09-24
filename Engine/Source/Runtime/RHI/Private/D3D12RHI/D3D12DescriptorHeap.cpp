// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12DescriptorHeap.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12ShaderResourceView.h"

ND3D12DescriptorHeap::ND3D12DescriptorHeap(ID3D12Device& InDevice) : pDevice(&InDevice)
{
	IncrementSize = (size_t)pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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

void ND3D12DescriptorHeap::Reserve(size_t InCount)
{
	if (InCount > Capacity)
	{
		Capacity = InCount;
		AllocateShaderVisibleHeap();
	}
}

void ND3D12DescriptorHeap::ApplyViewSimple(size_t Index, NRHIShaderResourceView& SourceView, size_t SourceIndex, size_t SourceRange)
{
	D3D12_CPU_DESCRIPTOR_HANDLE hSrcView = static_cast<ND3D12ShaderResourceView&>(SourceView).Get()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE hDestView = pShaderVisibleHeap->GetCPUDescriptorHandleForHeapStart();

	hSrcView.ptr += IncrementSize * SourceIndex;
	hDestView.ptr += IncrementSize * Index;

	pDevice->CopyDescriptorsSimple((UINT)SourceRange, hDestView, hSrcView, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

int64 ND3D12DescriptorHeap::GetVirtualHandleLocation(size_t Index) const
{
	D3D12_GPU_DESCRIPTOR_HANDLE hView = pShaderVisibleHeap->GetGPUDescriptorHandleForHeapStart();
	return hView.ptr + (int64)(IncrementSize * Index);
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