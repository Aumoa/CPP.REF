// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Buffer.h"
#include "D3D12CommandList.h"

GENERATE_BODY(SD3D12Buffer);

SD3D12Buffer::SD3D12Buffer(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12Resource> InResource, ComPtr<ID3D12Resource> InUploadHeap, const RHIBufferDesc& InDesc)
	: Super(InFactory, InDevice, InResource.Get(), InUploadHeap.Get())
	, Resource(std::move(InResource))
	, UploadHeap(std::move(InUploadHeap))
	, Desc(InDesc)
{
	if (UploadHeap)
	{
		TotalBytes = (uint64)InResource->GetDesc().Width;
	}
}

void SD3D12Buffer::UpdateSubresource(SD3D12CommandList* InCommandList, uint32 InSubresource, const RHISubresourceData* InUploadData)
{
	if (TotalBytes)
	{
		D3D12_RESOURCE_BARRIER Barrier = {};
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Transition.pResource = Resource.Get();
		Barrier.Transition.StateBefore = (D3D12_RESOURCE_STATES)Desc.InitialState;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		Barrier.Transition.Subresource = 0;

		InCommandList->ResourceBarrier(1, &Barrier);
		UpdateSubresource(InCommandList, Resource.Get(), UploadHeap.Get(), InSubresource, TotalBytes, InUploadData);

		std::swap(Barrier.Transition.StateBefore, Barrier.Transition.StateAfter);
		InCommandList->ResourceBarrier(1, &Barrier);
	}
	else
	{
		SE_LOG(LogDirectX, Error, L"Texture is not created with ERHIBufferUsage::Dynamic flags.");
	}
}

void SD3D12Buffer::UpdateSubresource(SD3D12CommandList* InCommandList, ID3D12Resource* InBuf, ID3D12Resource* InUploadBuf, uint32 InSubresource, uint64 InTotalBytes, const RHISubresourceData* InUploadData)
{
	int8* pData;
	HR(InUploadBuf->Map(0, nullptr, reinterpret_cast<void**>(&pData)));
	memcpy(pData, InUploadData->pSysMem, (size_t)InTotalBytes);
	InUploadBuf->Unmap(0, nullptr);

	InCommandList->CopyResource(InBuf, InUploadBuf);
}

void SD3D12Buffer::Dispose(bool bDisposing)
{
	Resource.Reset();
	UploadHeap.Reset();
	Super::Dispose(bDisposing);
}