// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12OnlineDescriptorPatch.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"
#include "D3D12ShaderResourceView.h"

D3D12OnlineDescriptorPatch::D3D12OnlineDescriptorPatch() : Super()
	, device(nullptr)

	, count(0)
	, seekpos(0)

	, handle_seek{ }
	, increment(0)
	, handle_start{ }
{
	device = Cast<D3D12DeviceBundle>(GEngine.DeviceBundle)->Device;
}

D3D12OnlineDescriptorPatch::~D3D12OnlineDescriptorPatch()
{

}

void D3D12OnlineDescriptorPatch::ReallocAndStart(size_t num)
{
	if (count < num)
	{
		// Realloc heap storage when new count is over than previous count.
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = { };
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = (uint32)num;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		HR(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap)));
		count = num;
	}

	seekpos = 0;
	handle_seek = heap->GetCPUDescriptorHandleForHeapStart();
	increment = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	handle_start = heap->GetGPUDescriptorHandleForHeapStart();
}

size_t D3D12OnlineDescriptorPatch::Patch(IRHIShaderResourceView* srv)
{
	auto seek = handle_seek.IncrementPost(increment, srv->Count);
	device->CopyDescriptorsSimple((uint32)srv->Count, seek, Cast<D3D12ShaderResourceView>(srv)->HandleBase, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	size_t left = seekpos;
	seekpos += srv->Count;
	return left;
}

D3D12_GPU_DESCRIPTOR_HANDLE D3D12OnlineDescriptorPatch::GetOnlineHandle(size_t seekpos) const
{
	return handle_start.Get(increment, seekpos);
}