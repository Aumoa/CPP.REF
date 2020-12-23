// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12CommandFence.h"

#include "Logging/LogMacros.h"
#include "D3D12ImmediateCommandList.h"

using namespace SC::Runtime::Game::RHI;
using namespace SC::Runtime::Game::D3D12RHI;

D3D12CommandFence::D3D12CommandFence(ID3D12Device* device) : Super()
	, fenceValue(0)
	, hEvent(nullptr)
{
	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	hEvent = CreateEventExW(nullptr, nullptr, 0, GENERIC_ALL);
}

D3D12CommandFence::~D3D12CommandFence()
{
	if (hEvent != nullptr)
	{
		CloseHandle(hEvent);
		hEvent = nullptr;
	}
}

void D3D12CommandFence::BeginFence()
{
	Wait();
}

void D3D12CommandFence::EndFence(IRHIImmediateCommandList* immediateCommandList)
{
	auto d3d12ImmediateCommandList = dynamic_cast<D3D12ImmediateCommandList*>(immediateCommandList);
	HR(d3d12ImmediateCommandList->CommandQueue->Signal(fence.Get(), ++fenceValue));
}

bool D3D12CommandFence::IsFenceCompleted() const
{
	return fence->GetCompletedValue() < fenceValue;
}

bool D3D12CommandFence::Wait() const
{
	if (fence->GetCompletedValue() < fenceValue)
	{
		HR(fence->SetEventOnCompletion(fenceValue, hEvent));
		DWORD waitCode = WaitForSingleObject(hEvent, 1000);

		if (waitCode == WAIT_TIMEOUT)
		{
			SE_LOG(LogD3D12RHI, Warning, L"Wait timeout detected in AutoFence");
			return false;
		}
		else if (waitCode != WAIT_OBJECT_0)
		{
			SE_LOG(LogD3D12RHI, Error, L"Cannot synchonize to fence. Wait function return {0}", (int32)waitCode);
			return false;
		}
	}

	return true;
}