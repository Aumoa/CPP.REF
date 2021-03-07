// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXAutoFence.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXCommandQueue.h"
#include "Windows/WinEvent.h"
#include "Time/StepTimer.h"

using namespace std;

DirectXAutoFence::DirectXAutoFence(DirectXDeviceBundle* deviceBundle, bool bBusyloop) : Super()
	, fenceValue(0)
	, bBusyloop(bBusyloop)
{
	ID3D12Device5* device = deviceBundle->GetDevice();
	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

	SetDeviceChildPtr(fence.Get(), deviceBundle);

	if (!bBusyloop)
	{
		winEvent = NewObject<WinEvent>();
	}
}

DirectXAutoFence::~DirectXAutoFence()
{

}

uint64 DirectXAutoFence::GetCompletedValue() const
{
	return fence->GetCompletedValue();
}

uint64 DirectXAutoFence::AcceptSignal(DirectXCommandQueue* queue)
{
	HR(queue->Item->Signal(fence.Get(), ++fenceValue));
	return fenceValue;
}

bool DirectXAutoFence::Wait(optional<Seconds> timeout) const
{
	if (bBusyloop)
	{
		return Wait_Busyloop(timeout);
	}
	else
	{
		return Wait_Event(timeout);
	}
}

bool DirectXAutoFence::Wait_Busyloop(optional<Seconds> timeout) const
{
	if (timeout.has_value())
	{
		StepTimer scopedTimer;
		while (fence->GetCompletedValue() < fenceValue)
		{
			scopedTimer.Tick();
			if (scopedTimer.ElapsedSeconds >= timeout.value())
			{
				// TIMEOUT
				return false;
			}
		}
		return true;
	}
	else
	{
		while (fence->GetCompletedValue() < fenceValue);
		return true;
	}
}

bool DirectXAutoFence::Wait_Event(optional<Seconds> timeout) const
{
	if (fence->GetCompletedValue() < fenceValue)
	{
		HR(fence->SetEventOnCompletion(fenceValue, winEvent->Handle));
		return winEvent->Wait(timeout);
	}

	return true;
}