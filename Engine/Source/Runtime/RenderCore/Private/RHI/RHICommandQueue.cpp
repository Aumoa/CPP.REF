// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHICommandQueue.h"
#include "InternalComPtr.h"
#include "RHI/LogRHI.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHIDeviceContext.h"

SRHICommandQueue::SRHICommandQueue(SRHIDevice* device, ERHICommandType commandType) : Super(device)
{
	ID3D12Device* d3ddev = device->GetDevice();

	D3D12_COMMAND_QUEUE_DESC desc =
	{
		.Type = (D3D12_COMMAND_LIST_TYPE)commandType
	};

	HR(LogRHI, d3ddev->CreateCommandQueue(&desc, IID_PPV_ARGS(&_queue)));
	HR(LogRHI, d3ddev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));

	_fenceEvent = NewObject<SEventHandle>();
}

SRHICommandQueue::~SRHICommandQueue()
{
	WaitLastSignal();
	Collect();
}

uint64 SRHICommandQueue::Signal()
{
	uint64 fenceValue = ++_signalNumber;
	HR(LogRHI, _queue->Signal(_fence.Get(), fenceValue));
	return fenceValue;
}

void SRHICommandQueue::WaitSignal(uint64 signalNumber)
{
	if (_fence->GetCompletedValue() < _signalNumber)
	{
		HR_E(LogRHI, _fence->SetEventOnCompletion(_signalNumber, _fenceEvent->GetHandle()));
		_fenceEvent->Wait();
	}
}

void SRHICommandQueue::WaitLastSignal()
{
	WaitSignal(_signalNumber);
}

uint64 SRHICommandQueue::GetLastSignal() const
{
	return _signalNumber;
}

uint64 SRHICommandQueue::ExecuteDeviceContexts(std::span<SRHIDeviceContext*> deviceContexts)
{
	std::vector<ID3D12CommandList*> commandLists;
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
	uint64 fenceValue = Signal();

	for (size_t i = 0; i < deviceContexts.size(); ++i)
	{
		deviceContexts[i]->FlushPendingDestroyObjects(fenceValue, this);
	}

	return fenceValue;
}

int32 SRHICommandQueue::Collect()
{
	uint64 fenceValue = _signalNumber;
	int32 count = 0;

	while (!_gcobjects.empty())
	{
		GarbageItem& item = _gcobjects.front();
		if (item.FenceValue > fenceValue)
		{
			return count;
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

		++count;
	}

	return count;
}

void SRHICommandQueue::AddGarbageObject(uint64 fenceValue, SObject* object)
{
	object->SetOuter(this);

	_gcobjects.emplace() =
	{
		.TypeIndex = 1,
		.FenceValue = fenceValue,
		.IsObject = object
	};
}

void SRHICommandQueue::AddGarbageObject(uint64 fenceValue, IUnknown* unknown)
{
	_gcobjects.emplace() =
	{
		.TypeIndex = 0,
		.FenceValue = fenceValue,
		.IsUnknown = unknown
	};
}