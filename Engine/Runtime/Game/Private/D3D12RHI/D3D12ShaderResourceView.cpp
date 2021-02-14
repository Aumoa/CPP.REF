// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12ShaderResourceView.h"

#include "D3D12OnlineDescriptorPatch.h"

D3D12ShaderResourceView::D3D12ShaderResourceView()
{

}

D3D12ShaderResourceView::~D3D12ShaderResourceView()
{

}

D3D12SingleShaderResourceViewNode::D3D12SingleShaderResourceViewNode(ID3D12Resource* resource, const D3D12OfflineDescriptorIndex& handle) : Super()
	, handle(handle)
{

}

D3D12SingleShaderResourceViewNode::~D3D12SingleShaderResourceViewNode()
{
	handle.Free();
}

size_t D3D12SingleShaderResourceViewNode::Count_get() const
{
	return 1;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12SingleShaderResourceViewNode::HandleBase_get() const
{
	return handle.Handle;
}

////////////////////////////////////////////////////////////////////////////////////////////

D3D12IndependentShaderResourceView::D3D12IndependentShaderResourceView(ID3D12Device* device, size_t numSRVs) : Super()
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

D3D12IndependentShaderResourceView::~D3D12IndependentShaderResourceView()
{

}

void D3D12IndependentShaderResourceView::CreateView(size_t index, ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle = handleBase.Get(increment, index);
	device->CreateShaderResourceView(resource, srvDesc, handle);
}

size_t D3D12IndependentShaderResourceView::Count_get() const
{
	return numSRVs;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12IndependentShaderResourceView::HandleBase_get() const
{
	return handleBase;
}