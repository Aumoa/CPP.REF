// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12CommandQueue.h"
#include "Threading/EventHandle.h"

SD3D12CommandQueue::SD3D12CommandQueue(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12CommandQueue> queue, ComPtr<ID3D12Fence> fence) : Super(factory, device)
	, _queue(std::move(queue))
	, _fence(std::move(fence))
{
	_event = NewObject<SEventHandle>();
}

void SD3D12CommandQueue::ExecuteCommandLists(std::span<IRHIDeviceContext*> deviceContexts)
{
	std::vector<ID3D12CommandList*> commandLists(deviceContexts.size());
	++_fenceValue;

	for (size_t i = 0; i < deviceContexts.size(); ++i)
	{
		auto deviceContext_s = Cast<SD3D12CommandList>(deviceContexts[i]);
		commandLists[i] = deviceContext_s->Get<ID3D12CommandList>();
		
		GC_Pending gc;
		gc.MarkedValue = _fenceValue;
		gc.Objects = deviceContext_s->ClearPendingObjects();
		_gc.emplace(std::move(gc));
	}

	_queue->ExecuteCommandLists((UINT)commandLists.size(), commandLists.data());
	_queue->Signal(_fence.Get(), _fenceValue);
}

uint64 SD3D12CommandQueue::GetFenceValue()
{
	return _fenceValue;
}

void SD3D12CommandQueue::Collect()
{
	while (!_gc.empty())
	{
		auto& front = _gc.front();
		if (front.MarkedValue <= _fenceValue)
		{
			for (size_t i = 0; i < front.Objects.size(); ++i)
			{
				DestroySubobject(front.Objects[i]);
			}

			_gc.pop();
		}
		else
		{
			break;
		}
	}
}