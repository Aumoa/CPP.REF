// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12CommandQueue.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12CommandSet.h"

ND3D12CommandQueue::ND3D12CommandQueue(ND3D12Graphics* InGraphics)
{
	ID3D12Device* Dev = InGraphics->GetDevice();

	D3D12_COMMAND_QUEUE_DESC QueueDesc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = (INT)D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
		.NodeMask = 0
	};
	HR(Dev->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&Queue)));
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