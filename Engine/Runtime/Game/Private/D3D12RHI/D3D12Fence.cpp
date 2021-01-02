// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12Fence.h"

#include "Engine.h"
#include "D3D12ImmediateCommandList.h"
#include "D3D12DeviceBundle.h"

D3D12Fence::D3D12Fence()
{
	ID3D12Device* dev = Cast<D3D12DeviceBundle>(GEngine.DeviceBundle)->Device;
	HR(dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
}

D3D12Fence::~D3D12Fence()
{

}

uint64 D3D12Fence::GetCompletedValue() const
{
	return fence->GetCompletedValue();
}

void D3D12Fence::Signal(IRHIImmediateCommandList* queue, uint64 number)
{
	HR(Cast<D3D12ImmediateCommandList>(queue)->CommandQueue->Signal(fence.Get(), number));
}