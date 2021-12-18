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
		std::vector<SharedPtr<SObject>> Objects;
		uint64 MarkedValue;
	};

	ComPtr<ID3D12CommandQueue> _queue;
	ComPtr<ID3D12Fence> _fence;
	uint64 _fenceValue = 0;
	SEventHandle* _event = nullptr;

	std::queue<GC_Pending> _gc;

public:
	SD3D12CommandQueue(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12CommandQueue> queue, ComPtr<ID3D12Fence> fence);

	using Super::Dispose;
	virtual void End() override;
	virtual uint64 ExecuteCommandLists(std::span<IRHIDeviceContext*> deviceContexts, bool bSignal) override;

	uint64 GetFenceValue();
	uint64 GetCompletedValue();
	void Collect();
	void WaitCompleted();

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID3D12CommandQueue, _queue);
	DECLARE_GETTER(ID3D12Fence, _fence);
};