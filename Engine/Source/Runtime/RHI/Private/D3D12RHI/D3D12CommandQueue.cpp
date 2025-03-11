// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12CommandQueue.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12CommandSet.h"

ND3D12CommandQueue::ND3D12CommandQueue(ComPtr<ID3D12CommandQueue> pQueue)
	: Queue(std::move(pQueue))
{
}

ND3D12CommandQueue::~ND3D12CommandQueue() noexcept
{
}

void ND3D12CommandQueue::ExecuteCommandSets(std::span<NRHICommandSet const* const> InCommandSets)
{
	static thread_local std::vector<ID3D12CommandList*> CachedVector;

	if (InCommandSets.empty())
	{
		return;
	}

	CachedVector.clear();
	CachedVector.reserve(InCommandSets.size());

	for (auto& CommandSet : InCommandSets)
	{
		CachedVector.emplace_back(static_cast<const ND3D12CommandSet*>(CommandSet)->GetCommandList());
	}

	Queue->ExecuteCommandLists((UINT)CachedVector.size(), CachedVector.data());
}

#endif