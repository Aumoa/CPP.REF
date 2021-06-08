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
	WaitLastSignal();
	Collect();
}

uint64 RHICommandQueue::Signal()
{
	uint64 fenceValue = ++_signalNumber;
	HR(LogRHI, _queue->Signal(_fence.Get(), fenceValue));
	return fenceValue;
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

uint64 RHICommandQueue::ExecuteDeviceContexts(span<RHIDeviceContext*> deviceContexts)
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
	return Signal();
}

void RHICommandQueue::Collect()
{
	uint64 fenceValue = _signalNumber;

	while (!_gcobjects.empty())
	{
		GarbageItem& item = _gcobjects.front();
		if (item.FenceValue > fenceValue)
		{
			return;
		}

		_gcobjects.pop();
		switch (item.TypeIndex)
		{
		case 0:
			item.IsUnknown->Release();
			break;
		case 1:
			DestroySubobject(item.IsObject);
			break;
		}
	}
}

void RHICommandQueue::AddGarbageObject(uint64 fenceValue, Object* object)
{
	object->SetOuter(this);

	_gcobjects.emplace() =
	{
		.TypeIndex = 1,
		.FenceValue = fenceValue,
		.IsObject = object
	};
}

void RHICommandQueue::AddGarbageObject(uint64 fenceValue, IUnknown* unknown)
{
	_gcobjects.emplace() =
	{
		.TypeIndex = 0,
		.FenceValue = fenceValue,
		.IsUnknown = unknown
	};
}