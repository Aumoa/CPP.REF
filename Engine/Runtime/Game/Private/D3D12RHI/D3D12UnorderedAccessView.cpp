// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12UnorderedAccessView.h"

#include "D3D12OnlineDescriptorPatch.h"

D3D12UnorderedAccessView::D3D12UnorderedAccessView()
{

}

D3D12UnorderedAccessView::~D3D12UnorderedAccessView()
{

}

D3D12SingleUnorderedAccessViewNode::D3D12SingleUnorderedAccessViewNode(ID3D12Resource* resource, const D3D12OfflineDescriptorIndex& handle) : Super()
	, handle(handle)
{

}

D3D12SingleUnorderedAccessViewNode::~D3D12SingleUnorderedAccessViewNode()
{
	handle.Free();
}

size_t D3D12SingleUnorderedAccessViewNode::Count_get() const
{
	return 1;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12SingleUnorderedAccessViewNode::HandleBase_get() const
{
	return handle.Handle;
}

////////////////////////////////////////////////////////////////////////////////////////////

D3D12IndependentUnorderedAccessView::D3D12IndependentUnorderedAccessView(ID3D12Device* device, size_t numSRVs) : Super()
	, device(device)
	, numSRVs(numSRVs)
	, handleBase{ }
	, increment(0)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = { };
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = (uint32)numSRVs;
	HR(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap)));

	handleBase = heap->GetCPUDescriptorHandleForHeapStart();
	increment = device->GetDescriptorHandleIncrementSize(desc.Type);
}

D3D12IndependentUnorderedAccessView::~D3D12IndependentUnorderedAccessView()
{

}

void D3D12IndependentUnorderedAccessView::CreateView(size_t index, ID3D12Resource* resource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = handleBase.Get(increment, index);
	device->CreateUnorderedAccessView(resource, nullptr, uavDesc, handle);
}

size_t D3D12IndependentUnorderedAccessView::Count_get() const
{
	return numSRVs;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12IndependentUnorderedAccessView::HandleBase_get() const
{
	return handleBase;
}