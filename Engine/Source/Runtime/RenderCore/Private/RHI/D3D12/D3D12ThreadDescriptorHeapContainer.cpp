// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12ThreadDescriptorHeapContainer.h"
#include "D3D12DescriptorHeap.h"

SD3D12ThreadDescriptorHeapContainer::SD3D12ThreadDescriptorHeapContainer(int64 threadId, ID3D12Device* device) : Super()
	, _threadId(threadId)
	, _device(device)
{
}

SD3D12DescriptorHeap* SD3D12ThreadDescriptorHeapContainer::GetUsableHeap(uint64 fenceValue, int32 reserveCount)
{
	if (auto& pendingBody = _heaps.front(); pendingBody.FenceValue >= fenceValue)
	{
		return GetSuitableHeap(pendingBody, reserveCount);
	}
	else
	{
		NewPendingHeap();
		return GetSuitableHeap(_heaps.front(), reserveCount);
	}
}

void SD3D12ThreadDescriptorHeapContainer::MarkPendingHeap(uint64 fenceValue)
{
	auto pendingBody = std::move(_heaps.front());
	_heaps.pop();

	pendingBody.FenceValue = fenceValue;
	_heaps.emplace(std::move(pendingBody));
}

void SD3D12ThreadDescriptorHeapContainer::NewPendingHeap()
{
	_heaps.emplace();
}

SD3D12DescriptorHeap* SD3D12ThreadDescriptorHeapContainer::GetSuitableHeap(HeapPendingBody& pendingBody, uint32 reserveCount)
{
	int32 minimumSlack = std::numeric_limits<int32>::max();
	SD3D12DescriptorHeap* suitableHeap = nullptr;
	for (size_t i = 0; i < pendingBody.Heaps.size(); ++i)
	{
		SD3D12DescriptorHeap* heap = pendingBody.Heaps[i];
		if (heap->Slack() >= (int32)reserveCount)
		{
			int32 updatedSlack = heap->Slack() - reserveCount;
			if (minimumSlack > updatedSlack)
			{
				minimumSlack = updatedSlack;
				suitableHeap = pendingBody.Heaps[i];
			}
		}
	}

	if (suitableHeap == nullptr)
	{
		suitableHeap = NewObject<SD3D12DescriptorHeap>(_device, CalcReserveCount(reserveCount));
		pendingBody.Heaps.emplace_back(suitableHeap);
	}

	return suitableHeap;
}