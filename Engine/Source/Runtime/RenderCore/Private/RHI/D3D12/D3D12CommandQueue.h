// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12CommandList.h"
#include <queue>

class SEventHandle;

class SD3D12CommandQueue : public SD3D12CommandList
{
	GENERATED_BODY(SD3D12CommandQueue)

private:
	struct GC_Pending
	{
		std::vector<SObject*> Objects;
		uint64 MarkedValue;
	};

	ComPtr<ID3D12CommandQueue> _queue;
	ComPtr<ID3D12Fence> _fence;
	uint64 _fenceValue = 0;
	SEventHandle* _event = nullptr;

	std::queue<GC_Pending> _gc;

public:
	SD3D12CommandQueue(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12CommandQueue> queue, ComPtr<ID3D12Fence> fence);
	virtual ~SD3D12CommandQueue() override;

	virtual void ExecuteCommandLists(std::span<IRHIDeviceContext*> deviceContexts) override;

	uint64 GetFenceValue();
	void Collect();
	void WaitCompleted();

public:
	DECLARE_GETTER(ID3D12CommandQueue, _queue);
	DECLARE_GETTER(ID3D12Fence, _fence);
};