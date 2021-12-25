// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12CommandList.h"
#include <queue>

class SEventHandle;

class DIRECTX_API SD3D12CommandQueue : public SD3D12CommandList
{
	GENERATED_BODY(SD3D12CommandQueue)

private:
	struct LockedGarbages
	{
		uint64 MarkedValue = 0;
		std::vector<SharedPtr<IDisposable>> Objects;
	};

	ComPtr<ID3D12CommandQueue> Queue;
	ComPtr<ID3D12Fence> Fence;
	uint64 FenceValue = 0;

	SPROPERTY(Event)
	SEventHandle* Event = nullptr;
	std::queue<LockedGarbages> PendingGarbages;

public:
	SD3D12CommandQueue(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12CommandQueue> Queue, ComPtr<ID3D12Fence> Fence);

	virtual void End() override;
	virtual uint64 ExecuteCommandLists(std::span<IRHIDeviceContext*> DeviceContexts, bool bSignal) override;

	uint64 GetFenceValue();
	uint64 GetCompletedValue();
	void Collect();
	void WaitCompleted();

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID3D12CommandQueue, Queue);
	DECLARE_GETTER(ID3D12Fence, Fence);
};