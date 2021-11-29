// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Buffer.h"
#include "D3D12CommandList.h"

SD3D12Buffer::SD3D12Buffer(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadHeap, const RHIBufferDesc& desc) : Super(factory, device, resource, uploadHeap)
	, _resource(std::move(resource))
	, _uploadHeap(std::move(uploadHeap))
	, _desc(desc)
{
	if (_uploadHeap)
	{
		_totalBytes = (uint64)resource->GetDesc().Width;
	}
}

void SD3D12Buffer::UpdateSubresource(SD3D12CommandList* commandList, uint32 subresource, const RHISubresourceData* uploadData)
{
	if (_totalBytes)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = _resource.Get();
		barrier.Transition.StateBefore = (D3D12_RESOURCE_STATES)_desc.InitialState;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.Subresource = 0;

		commandList->ResourceBarrier(1, &barrier);
		UpdateSubresource(commandList, _resource.Get(), _uploadHeap.Get(), subresource, _totalBytes, uploadData);

		std::swap(barrier.Transition.StateBefore, barrier.Transition.StateAfter);
		commandList->ResourceBarrier(1, &barrier);
	}
	else
	{
		SE_LOG(LogWindows, Error, L"Texture is not created with ERHIBufferUsage::Dynamic flags.");
	}
}

void SD3D12Buffer::UpdateSubresource(SD3D12CommandList* commandList, ID3D12Resource* buf, ID3D12Resource* uploadBuf, uint32 subresource, uint64 totalBytes, const RHISubresourceData* uploadData)
{
	int8* pData;
	HR(uploadBuf->Map(0, nullptr, reinterpret_cast<void**>(&pData)));
	memcpy(pData, uploadData->pSysMem, (size_t)totalBytes);
	uploadBuf->Unmap(0, nullptr);

	commandList->CopyResource(buf, uploadBuf);
}