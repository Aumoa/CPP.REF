// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12ThreadAllocatorContainer.h"

SD3D12ThreadAllocatorContainer::SD3D12ThreadAllocatorContainer(int64 threadId, ID3D12Device* device) : Super()
	, _threadId(threadId)
	, _device(device)
{
	// Create primary allocator for fast using.
	NewAllocator();
}

ID3D12CommandAllocator* SD3D12ThreadAllocatorContainer::GetPrimaryAllocator(uint64 fenceValue)
{
	if (auto& pendingBody = _allocators.front(); pendingBody.FenceValue <= fenceValue)
	{
		ID3D12CommandAllocator* Allocator = pendingBody.IncrementGetAllocator(this);
		if (pendingBody.FenceValue != 0)
		{
			for (auto& Alloc : pendingBody.Allocators)
			{
				HR(Alloc->Reset());
			}
			pendingBody.FenceValue = 0;
		}

		return Allocator;
	}
	else
	{
		NewAllocator();
		return _allocators.front().IncrementGetAllocator(this);
	}
}

void SD3D12ThreadAllocatorContainer::MarkPendingAllocator(uint64 fenceValue)
{
	auto pendingBody = std::move(_allocators.front());
	_allocators.pop();

	pendingBody.FenceValue = fenceValue;
	pendingBody.AllocatorIndex = 0;
	_allocators.emplace(std::move(pendingBody));
}

void SD3D12ThreadAllocatorContainer::NewAllocator(AllocatorPendingBody& InBody)
{
	HR(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&InBody.Allocators.emplace_back())));
}

void SD3D12ThreadAllocatorContainer::NewAllocator()
{
	AllocatorPendingBody NewBody = _allocators.emplace();
	NewBody.FenceValue = 0;
	NewBody.AllocatorIndex = 0;
	NewAllocator(NewBody);
}