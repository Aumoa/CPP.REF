// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXCommandQueue.h"
#include "DirectXSwapChain.h"
#include "DirectXFence.h"
#include "DirectXCommandList.h"

GENERATE_BODY(SDirectXCommandQueue);

SDirectXCommandQueue::SDirectXCommandQueue(IRHIDevice* Owner, ComPtr<ID3D12CommandQueue> pQueue)
	: Super(Owner)
	, pQueue(std::move(pQueue))
{
}

void SDirectXCommandQueue::ExecuteCommandBuffers(std::span<IRHIGraphicsCommandList* const> commandLists)
{
	std::vector<ID3D12CommandList*> l_commandLists;
	ReplaceNativePointer(l_commandLists, commandLists);
	pQueue->ExecuteCommandLists((UINT)l_commandLists.size(), l_commandLists.data());
}

void SDirectXCommandQueue::Signal(IRHIFence* pFence, uint64 fenceValue)
{
	HR(pQueue->Signal(Cast<SDirectXFence>(pFence)->pFence.Get(), fenceValue));
}

void SDirectXCommandQueue::Dispose(bool bDisposing)
{
	pQueue.Reset();
	Super::Dispose(bDisposing);
}