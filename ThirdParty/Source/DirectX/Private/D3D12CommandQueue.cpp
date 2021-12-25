// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12CommandQueue.h"
#include "Threading/EventHandle.h"

GENERATE_BODY(SD3D12CommandQueue);

SD3D12CommandQueue::SD3D12CommandQueue(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12CommandQueue> queue, ComPtr<ID3D12Fence> fence) : Super(InFactory, InDevice)
	, _queue(std::move(queue))
	, _fence(std::move(fence))
{
	_event = gcnew SEventHandle();
}

void SD3D12CommandQueue::End()
{
	Super::End();
	ExecuteCommandList(this);
}

uint64 SD3D12CommandQueue::ExecuteCommandLists(std::span<IRHIDeviceContext*> deviceContexts, bool bSignal)
{
	std::vector<ID3D12CommandList*> commandLists;
	commandLists.reserve(deviceContexts.size());

	++_fenceValue;

	for (size_t i = 0; i < deviceContexts.size(); ++i)
	{
		auto deviceContext_s = Cast<SD3D12CommandList>(deviceContexts[i]);
		if (deviceContext_s)
		{
			if (auto commandList = deviceContext_s->Get<ID3D12CommandList>(); commandList)
			{
				commandLists.emplace_back(commandList);

				std::vector<SObject*> Objects = deviceContext_s->ClearPendingObjects();

				GC_Pending Pending;
				Pending.MarkedValue = _fenceValue;
				Pending.Objects.reserve(Objects.size());

				for (auto& Obj : Objects)
				{
					Pending.Objects.emplace_back(Obj);
				}

				_gc.emplace(std::move(Pending));
			}
		}
	}

	if (commandLists.size())
	{
		_queue->ExecuteCommandLists((UINT)commandLists.size(), commandLists.data());
	}

	if (bSignal)
	{
		_queue->Signal(_fence.Get(), _fenceValue);
		return _fenceValue;
	}
	else
	{
		return --_fenceValue;
	}
}

uint64 SD3D12CommandQueue::GetFenceValue()
{
	return _fenceValue;
}

uint64 SD3D12CommandQueue::GetCompletedValue()
{
	return _fence->GetCompletedValue();
}

void SD3D12CommandQueue::Collect()
{
	while (!_gc.empty())
	{
		auto& front = _gc.front();
		if (front.MarkedValue <= _fence->GetCompletedValue())
		{
			for (size_t i = 0; i < front.Objects.size(); ++i)
			{
				SObject* Object = front.Objects[i].Get();
				if (auto* Disp = Cast<IDisposable>(Object))
				{
					Disp->Dispose();
				}
				else
				{
					GC.SuppressFinalize(Object);
				}
			}

			_gc.pop();
		}
		else
		{
			break;
		}
	}
}

void SD3D12CommandQueue::WaitCompleted()
{
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _event->GetHandle());
		_event->Wait();
	}
}

void SD3D12CommandQueue::Dispose(bool bDisposing)
{
	WaitCompleted();

	if (bDisposing)
	{
		_queue.Reset();
		_fence.Reset();

		std::queue<GC_Pending> Swap;
		std::swap(Swap, _gc);
	}

	Super::Dispose(bDisposing);
}