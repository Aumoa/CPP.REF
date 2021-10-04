// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DescriptorHeap.h"

SD3D12DescriptorHeap::SD3D12DescriptorHeap(ID3D12Device* device, int32 count) : Super()
	, _device(device)
	, _count(count)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = (UINT)count;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HR(_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_heap)));
}

void SD3D12DescriptorHeap::MarkPendingHeap()
{
	_index = 0;
}

int32 SD3D12DescriptorHeap::Increment(int32 count)
{
	int32 index = _index;
	_index += count;
	return index;
}

int32 SD3D12DescriptorHeap::Slack()
{
	return _count - _index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SD3D12DescriptorHeap::GetCPUHandle(std::optional<int32> index)
{
	if (!index.has_value()) { index = _index; }
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _handleCPU;
	handle.ptr += (SIZE_T)_incrementSize * *index;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE SD3D12DescriptorHeap::GetGPUHandle(std::optional<int32> index)
{
	if (!index.has_value()) { index = _index; }
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _handleGPU;
	handle.ptr += (UINT64)_incrementSize * *index;
	return handle;
}