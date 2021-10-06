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
		if (pendingBody.FenceValue != 0)
		{
			HR(pendingBody.Allocator->Reset());
			pendingBody.FenceValue = 0;
		}

		return pendingBody.Allocator.Get();
	}
	else
	{
		NewAllocator();
		return _allocators.front().Allocator.Get();
	}
}

void SD3D12ThreadAllocatorContainer::MarkPendingAllocator(uint64 fenceValue)
{
	auto pendingBody = std::move(_allocators.front());
	_allocators.pop();

	pendingBody.FenceValue = fenceValue;
	_allocators.emplace(std::move(pendingBody));
}

void SD3D12ThreadAllocatorContainer::NewAllocator()
{
	HR(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_allocators.emplace().Allocator)));
}