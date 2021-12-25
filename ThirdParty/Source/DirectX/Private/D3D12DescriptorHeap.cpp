// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DescriptorHeap.h"

GENERATE_BODY(SD3D12DescriptorHeap);

SD3D12DescriptorHeap::SD3D12DescriptorHeap(ID3D12Device* InDevice, int32 Count) : Super()
	, Device(InDevice)
	, Count(Count)
{
	D3D12_DESCRIPTOR_HEAP_DESC Desc = {};
	Desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	Desc.NumDescriptors = (UINT)Count;
	Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HR(Device->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&Heap)));

	HandleCPU = Heap->GetCPUDescriptorHandleForHeapStart();
	HandleGPU = Heap->GetGPUDescriptorHandleForHeapStart();
}

void SD3D12DescriptorHeap::MarkPendingHeap()
{
	Index = 0;
}

int32 SD3D12DescriptorHeap::Increment(int32 Count)
{
	int32 MyIndex = Index;
	Index += Count;
	return MyIndex;
}

int32 SD3D12DescriptorHeap::Slack()
{
	return Count - Index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SD3D12DescriptorHeap::GetCPUHandle(std::optional<int32> InIndex)
{
	if (!InIndex.has_value()) { InIndex = Index; }
	D3D12_CPU_DESCRIPTOR_HANDLE Handle = HandleCPU;
	Handle.ptr += (SIZE_T)IncrementSize * *InIndex;
	return Handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SD3D12DescriptorHeap::GetGPUHandle(std::optional<int32> InIndex)
{
	if (!InIndex.has_value()) { InIndex = Index; }
	D3D12_GPU_DESCRIPTOR_HANDLE Handle = HandleGPU;
	Handle.ptr += (UINT64)IncrementSize * *InIndex;
	return Handle;
}

void SD3D12DescriptorHeap::Dispose(bool bDisposing)
{
	Device = nullptr;
	Heap.Reset();
	HandleCPU = {};
	HandleGPU = {};

	Super::Dispose(bDisposing);
}