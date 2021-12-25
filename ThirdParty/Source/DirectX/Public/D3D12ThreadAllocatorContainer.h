// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXCommon.h"
#include <queue>

class DIRECTX_API SD3D12ThreadAllocatorContainer : implements SObject
{
	GENERATED_BODY(SD3D12ThreadAllocatorContainer)

private:
	struct AllocatorPendingBody
	{
		std::vector<ComPtr<ID3D12CommandAllocator>> Allocators;
		uint64 FenceValue;
		size_t AllocatorIndex;

		ID3D12CommandAllocator* IncrementGetAllocator(SD3D12ThreadAllocatorContainer* This)
		{
			if (Allocators.size() <= AllocatorIndex)
			{
				This->NewAllocator(*this);
			}
			return Allocators[AllocatorIndex++].Get();
		}
	};

private:
	int64 _threadId = 0;
	ID3D12Device* _device = nullptr;
	AllocatorPendingBody _body;
	std::queue<AllocatorPendingBody> _allocators;

public:
	SD3D12ThreadAllocatorContainer(int64 threadId, ID3D12Device* device);

	ID3D12CommandAllocator* GetPrimaryAllocator(uint64 fenceValue);
	void MarkPendingAllocator(uint64 fenceValue);

private:
	void NewAllocator(AllocatorPendingBody& InBody);
	void NewAllocator();
};