// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import SC.Runtime.RenderCore;
import SC.Runtime.Core;
import std.core;

using namespace std;

RHICommandQueue::RHICommandQueue(RHIDevice* device, ERHICommandType commandType) : Super(device)
{
	ID3D12Device* d3ddev = device->GetDevice();

	D3D12_COMMAND_QUEUE_DESC desc =
	{
		.Type = (D3D12_COMMAND_LIST_TYPE)commandType
	};

	HR(LogRHI, d3ddev->CreateCommandQueue(&desc, IID_PPV_ARGS(&_queue)));
	HR(LogRHI, d3ddev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));

	_fenceEvent = CreateSubobject<EventHandle>();
}

RHICommandQueue::~RHICommandQueue()
{
}

uint64 RHICommandQueue::Signal()
{
	HR(LogRHI, _queue->Signal(_fence.Get(), ++_signalNumber));
	return _signalNumber;
}

void RHICommandQueue::WaitSignal(uint64 signalNumber)
{
	if (_fence->GetCompletedValue() < _signalNumber)
	{
		HR_E(LogRHI, _fence->SetEventOnCompletion(_signalNumber, _fenceEvent->GetHandle()));
		_fenceEvent->Wait();
	}
}

void RHICommandQueue::WaitLastSignal()
{
	WaitSignal(_signalNumber);
}

void RHICommandQueue::ExecuteDeviceContexts(span<RHIDeviceContext*> deviceContexts)
{
	vector<ID3D12CommandList*> commandLists;
	commandLists.reserve(deviceContexts.size());

	for (size_t i = 0; i < deviceContexts.size(); ++i)
	{
		ID3D12CommandList* cmd = deviceContexts[i]->GetCommandList();
		if (cmd != nullptr)
		{
			commandLists.emplace_back(cmd);
		}
	}

	_queue->ExecuteCommandLists((UINT)commandLists.size(), commandLists.data());
}