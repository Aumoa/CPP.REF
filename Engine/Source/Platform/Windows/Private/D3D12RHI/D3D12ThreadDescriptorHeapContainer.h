// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXCommon.h"
#include <queue>

class SD3D12DescriptorHeap;

class SD3D12ThreadDescriptorHeapContainer : implements SObject
{
	GENERATED_BODY(SD3D12ThreadDescriptorHeapContainer)

private:
	struct HeapPendingBody
	{
		uint64 FenceValue;
		std::vector<SD3D12DescriptorHeap*> Heaps;
	};

private:
	int64 _threadId = 0;
	ID3D12Device* _device = nullptr;
	std::queue<HeapPendingBody> _heaps;

public:
	SD3D12ThreadDescriptorHeapContainer(int64 threadId, ID3D12Device* device);

	SD3D12DescriptorHeap* GetUsableHeap(uint64 fenceValue, int32 reserveCount);
	void MarkPendingHeap(uint64 fenceValue);

private:
	void NewPendingHeap();
	SD3D12DescriptorHeap* GetSuitableHeap(HeapPendingBody& pendingBody, uint32 reserveCount);

private:
	static constexpr uint32 CalcReserveCount(uint32 reserveCount)
	{
		return (reserveCount + 255) & ~255;
	}
};