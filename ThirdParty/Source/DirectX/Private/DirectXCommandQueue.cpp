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

int32 SDirectXCommandQueue::AcquireSwapChainImage(IRHISwapChain* SwapChain)
{
	auto sSwapChain = Cast<SDirectXSwapChain>(SwapChain);
	int32 Index = (int32)sSwapChain->pSwapChain->GetCurrentBackBufferIndex();
	return Index;
}

void SDirectXCommandQueue::Present(IRHISwapChain* SwapChain, int32 BufferIndex)
{
	auto sSwapChain = Cast<SDirectXSwapChain>(SwapChain);
	HR(sSwapChain->pSwapChain->Present(0, 0));
	HR(pQueue->Signal(sSwapChain->pFence.Get(), ++sSwapChain->FenceValue));
}

void SDirectXCommandQueue::Submit(std::span<IRHICommandBuffer* const> CommandBuffers, IRHIFence* Fence)
{
	std::vector<ID3D12CommandList*> pCommandLists;
	pCommandLists.reserve(CommandBuffers.size());

	for (auto& CommandBuffer : CommandBuffers)
	{
		auto sCommandBuffer = Cast<SDirectXCommandList>(CommandBuffer);
		pCommandLists.emplace_back(sCommandBuffer->pCommandList.Get());
	}

	pQueue->ExecuteCommandLists((UINT)pCommandLists.size(), pCommandLists.data());

	if (auto* sFence = Cast<SDirectXFence>(Fence); sFence)
	{
		ID3D12Fence* pFence = sFence->pFence.Get();
		UINT64 FenceValue = ++sFence->FenceValue;
		HR(pQueue->Signal(pFence, FenceValue));
	}
}

void SDirectXCommandQueue::Dispose(bool bDisposing)
{
	pQueue.Reset();
	Super::Dispose(bDisposing);
}